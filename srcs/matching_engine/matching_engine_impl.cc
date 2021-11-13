#include "srcs/matching_engine/matching_engine_impl.h"

#include "srcs/order/order.h"
#include "srcs/order/order_book_entry.h"

namespace fep::srcs::matching_engine
{
  namespace
  {
    using ::fep::lib::Price4;
    using ::fep::lib::TradeAction;
    using ::fep::lib::TradeMessage;
    using ::fep::lib::TradeResult;
    using ::fep::srcs::order::Order;
    using ::fep::srcs::order::OrderBook;
    using ::fep::srcs::order::OrderBookEntry;
    using ::fep::srcs::order::OrderPool;
    using ::fep::srcs::order::OrderSide;
    using ::fep::srcs::order::OrderStatus;
    using ::fep::srcs::order::OrderType;
    using ::fep::srcs::order::TimeInForce;
    using ::fep::srcs::stock::Symbol;

    using OrderId = int64_t;
    using TimestampSec = int64_t;

    void UpdateTradeUpdateForSeenPrices(const Symbol symbol, const OrderPool &order_pool, const std::set<Price4> &prices, std::vector<TradeAction> &trade_actions)
    {
      for (const auto &price : prices)
      {
        const int32_t quantity = order_pool.GetQuantityForPrice(symbol, price);
        trade_actions.push_back({TradeAction(price, quantity, (quantity == 0 ? fep::lib::kDeleteAction : fep::lib::kModifyAction))});
      }
    }

    template <class T>
    void MaybeUpdateInfoForNewOrder(OrderPool &order_pool, OrderBook<T> &order_book, std::unique_ptr<Order> order, std::vector<TradeAction> &trade_actions)
    {
      if (order->time_in_force() == TimeInForce::IOC || order->order_type() == OrderType::MARKET)
      {
        return;
      }
      if (order->quantity() != 0)
      {
        const Price4 price = order->price();
        const OrderId order_id = order->order_id();
        const TimestampSec timestamp_sec = order->timestamp_sec();
        const Symbol symbol = order->symbol();
        const int32_t pre_quantity = order_pool.GetQuantityForPrice(symbol, price);

        order_book.AddEntry(OrderBookEntry{
            .timestamp_sec = timestamp_sec,
            .order_id = order_id,
            .price = price});
        order_pool.AddOrder(std::move(order));
        const int32_t quantity = order_pool.GetQuantityForPrice(symbol, price);
        trade_actions.push_back({TradeAction(price, quantity, (pre_quantity == 0 ? fep::lib::kAddAction : fep::lib::kModifyAction))});
      }
    }

    template <class T>
    bool ProcessFirstOrderEntryAndContinue(const bool sell, const Order &first_order, Order &new_order, std::set<Price4> &prices_seen, OrderPool &order_pool, OrderBook<T> &order_book, std::vector<TradeResult> &trade_results)
    {
      bool is_offer_matched = (new_order.order_type() == OrderType::MARKET);
      if (new_order.order_type() == OrderType::LIMIT)
      {
        is_offer_matched = sell ? first_order.price() >= new_order.price() : first_order.price() <= new_order.price();
      }
      if (!is_offer_matched)
      {
        return false;
      }

      prices_seen.insert(first_order.price());

      if (first_order.quantity() > new_order.quantity())
      {
        trade_results.push_back({TradeResult(first_order.price(), new_order.quantity())});
        order_pool.ModifyOrder(first_order.order_id(), -new_order.quantity());
        new_order.set_quantity(0);
        return false;
      }

      trade_results.push_back({TradeResult(first_order.price(), first_order.quantity())});
      order_book.RemoveFirstEntry();
      order_pool.RemoveOrder(first_order.order_id());
      new_order.set_quantity(new_order.quantity() - first_order.quantity());
      return true;
    }
  }

  template <class T>
  bool MaybeUpdateInfoWhenCancellingOrder(const OrderId order_id, OrderPool &order_pool, OrderBook<T> &order_book, std::vector<TradeAction> &trade_actions)
  {
    const auto *matched_order = order_pool.GetOrder(order_id);
    if (matched_order == nullptr)
    {
      return false;
    }

    const TimestampSec timestamp_sec = matched_order->timestamp_sec();
    const Price4 price = matched_order->price();
    const Symbol symbol = matched_order->symbol();
    order_book.RemoveEntry(OrderBookEntry{.timestamp_sec = timestamp_sec, .order_id = order_id, .price = price});
    order_pool.RemoveOrder(order_id);
    const int32_t quantity = order_pool.GetQuantityForPrice(symbol, price);
    trade_actions.push_back({TradeAction(price, quantity, (quantity == 0 ? fep::lib::kDeleteAction : fep::lib::kModifyAction))});
    return true;
  }

  absl::StatusOr<TradeMessage> MatchingEngineImpl::Process(std::unique_ptr<Order> order)
  {
    absl::Status valid_status = order->is_valid_order(tick_size_rule_, lot_size_);
    if (!valid_status.ok())
    {
      return valid_status;
    }
    if (order->type() == OrderStatus::UNKNOWN)
    {
      return absl::InvalidArgumentError("not a NEW or CANCEL order");
    }
    if (order->type() == OrderStatus::CANCEL)
    {
      return Cancel(std::move(order));
    }
    if (order->side() == OrderSide::BUY)
    {
      return Buy(std::move(order));
    }
    if (order->side() == OrderSide::SELL)
    {
      return Sell(std::move(order));
    }
    return absl::InvalidArgumentError("order cannot be processed");
  }

  absl::StatusOr<TradeMessage> MatchingEngineImpl::Sell(std::unique_ptr<Order> order)
  {
    TradeMessage trade_message;
    std::set<Price4> prices_seen;
    const Symbol symbol = order->symbol();
    auto &bid_order_book = bid_order_books_[symbol];
    while (!bid_order_book.Empty() && order->quantity() != 0)
    {
      const auto &book_entry = bid_order_book.FirstEntry();
      const auto *first_order = bid_order_pool_.GetOrder(book_entry.order_id);
      if (first_order == nullptr)
      {
        return absl::NotFoundError("order_id not found in the order pool.");
      }

      if (!ProcessFirstOrderEntryAndContinue(/*sell=*/true, *first_order, *order, prices_seen, bid_order_pool_, bid_order_book, trade_message.trade_results))
      {
        break;
      }
    }

    MaybeUpdateInfoForNewOrder(ask_order_pool_, ask_order_books_[symbol], std::move(order), trade_message.trade_update.asks);
    UpdateTradeUpdateForSeenPrices(symbol, bid_order_pool_, prices_seen, trade_message.trade_update.bids);

    return trade_message;
  }

  absl::StatusOr<TradeMessage> MatchingEngineImpl::Buy(std::unique_ptr<Order> order)
  {
    TradeMessage trade_message;
    std::set<Price4> prices_seen;
    const Symbol symbol = order->symbol();
    auto &ask_order_book = ask_order_books_[symbol];
    while (!ask_order_book.Empty() && order->quantity() != 0)
    {
      const auto &book_entry = ask_order_book.FirstEntry();
      const auto *first_order = ask_order_pool_.GetOrder(book_entry.order_id);
      if (first_order == nullptr)
      {
        return absl::NotFoundError("order_id not found in the order pool.");
      }

      if (!ProcessFirstOrderEntryAndContinue(/*sell=*/false, *first_order, *order, prices_seen, ask_order_pool_, ask_order_book, trade_message.trade_results))
      {
        break;
      }
    }

    MaybeUpdateInfoForNewOrder(bid_order_pool_, bid_order_books_[symbol], std::move(order), trade_message.trade_update.bids);
    UpdateTradeUpdateForSeenPrices(symbol, ask_order_pool_, prices_seen, trade_message.trade_update.asks);

    return trade_message;
  }

  absl::StatusOr<TradeMessage> MatchingEngineImpl::Cancel(std::unique_ptr<Order> order)
  {
    const OrderId order_id = order->order_id();
    TradeMessage trade_message;
    if (MaybeUpdateInfoWhenCancellingOrder(order_id, bid_order_pool_, bid_order_books_[order->symbol()], trade_message.trade_update.bids) ||
        MaybeUpdateInfoWhenCancellingOrder(order_id, ask_order_pool_, ask_order_books_[order->symbol()], trade_message.trade_update.asks))
    {
      return trade_message;
    }
    return absl::NotFoundError(absl::StrCat("Failed to cancel order ", order_id));
  }

} // namespace fep::srcs::matching_engine