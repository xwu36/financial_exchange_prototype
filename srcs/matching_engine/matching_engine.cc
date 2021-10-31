#include "srcs/matching_engine/matching_engine.h"

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
    using ::fep::srcs::order::OrderBookEntry;
    using ::fep::srcs::order::OrderPool;
    using ::fep::srcs::order::OrderSide;

    void UpdateTradeUpdateForSeenPrices(const OrderPool &order_pool, const std::set<Price4> &prices, std::vector<TradeAction> &trade_actions)
    {
      for (const auto &price : prices)
      {
        const int32_t quantity = order_pool.GetQuantityForPrice(price);
        trade_actions.push_back({TradeAction(price, quantity, (quantity == 0 ? fep::lib::kDeleteAction : fep::lib::kModifyAction))});
      }
    }
  }

  absl::StatusOr<TradeMessage> MatchingEngine::Process(std::unique_ptr<Order> order)
  {
    if (order == nullptr || !order->is_valid_order())
    {
      return absl::InvalidArgumentError("order is not valid.");
    }
    if (order->side() == OrderSide::BUY) {
      return Buy(std::move(order));
    }
    if(order->side() == OrderSide::SELL) {
      return Sell(std::move(order));
    }
    return absl::InvalidArgumentError("");
  }

  absl::StatusOr<TradeMessage> MatchingEngine::Sell(std::unique_ptr<Order> order)
  {
    TradeMessage trade_message;
    std::set<Price4> prices_seen;
    while (!bid_order_book_.Empty() && order->quantity() != 0)
    {
      const auto &book_entry = bid_order_book_.FirstEntry();
      const auto *first_order = order_pool_.GetOrder(book_entry.order_id);
      if (first_order == nullptr)
      {
        return absl::NotFoundError("order_id not found in the order pool.");
      }

      if (first_order->price() >= order->price())
      {
        prices_seen.insert(first_order->price());

        if (first_order->quantity() > order->quantity())
        {
          trade_message.trade_results.push_back({TradeResult(first_order->price(), order->quantity())});
          order_pool_.ModifyOrder(first_order->order_id(), -order->quantity());
          break;
        }

        order->set_quantity(order->quantity() - first_order->quantity());

        bid_order_book_.RemoveFirstEntry();
        order_pool_.RemoveOrder(first_order->order_id());
      }
      else
      {
        break;
      }
    }

    if (order->quantity() != 0)
    {
      ask_order_book_.AddEntry(OrderBookEntry{
          .timestamp_sec = order->timestamp_sec(),
          .order_id = order->order_id(),
          .price = order->price()});
      trade_message.trade_update.asks.push_back({TradeAction(order->price(), order->quantity(), fep::lib::kAddAction)});
      order_pool_.AddOrder(std::move(order));
    }

    UpdateTradeUpdateForSeenPrices(order_pool_, prices_seen, trade_message.trade_update.bids);

    return trade_message;
  }

  absl::StatusOr<TradeMessage> MatchingEngine::Buy(std::unique_ptr<Order> order)
  {
    TradeMessage trade_message;
    std::set<Price4> prices_seen;
    while (!ask_order_book_.Empty() && order->quantity() != 0)
    {
      const auto &book_entry = ask_order_book_.FirstEntry();
      const auto *first_order = order_pool_.GetOrder(book_entry.order_id);
      if (first_order == nullptr)
      {
        return absl::NotFoundError("order_id not found in the order pool.");
      }

      if (first_order->price() <= order->price())
      {
        prices_seen.insert(first_order->price());

        if (first_order->quantity() > order->quantity())
        {
          trade_message.trade_results.push_back({TradeResult(first_order->price(), order->quantity())});
          order_pool_.ModifyOrder(first_order->order_id(), -order->quantity());
          break;
        }

        order->set_quantity(order->quantity() - first_order->quantity());

        ask_order_book_.RemoveFirstEntry();
        order_pool_.RemoveOrder(first_order->order_id());
      }
      else
      {
        break;
      }
    }

    if (order->quantity() != 0)
    {
      bid_order_book_.AddEntry(OrderBookEntry{
          .timestamp_sec = order->timestamp_sec(),
          .order_id = order->order_id(),
          .price = order->price()});
      trade_message.trade_update.bids.push_back({TradeAction(order->price(), order->quantity(), fep::lib::kAddAction)});
      order_pool_.AddOrder(std::move(order));
    }

    UpdateTradeUpdateForSeenPrices(order_pool_, prices_seen, trade_message.trade_update.asks);

    return trade_message;
  }

} // namespace fep::srcs::matching_engine