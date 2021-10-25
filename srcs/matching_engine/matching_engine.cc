#include "srcs/matching_engine/matching_engine.h"

#include "srcs/order/order.h"
#include "srcs/order/order_book_entry.h"

namespace fep::srcs::matching_engine
{
  using ::fep::srcs::order::Order;
  using ::fep::srcs::order::OrderBookEntry;

  void MatchingEngine::Sell(std::unique_ptr<Order> order)
  {
    while (!bid_order_book_.Empty() && order->quantity() != 0)
    {
      const auto &book_entry = bid_order_book_.FirstEntry();
      const auto &first_order = order_pool_.GetOrder(book_entry.order_id);
      if (first_order->price() >= order->price())
      {
        if (first_order->quantity() > order->quantity())
        {
          first_order->set_quantity(first_order->quantity() - order->quantity());
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
      order_pool_.AddOrder(std::move(order));
    }
  }

} // namespace fep::srcs::matching_engine