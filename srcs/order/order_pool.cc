#include "srcs/order/order_pool.h"

#include "absl/flags/flag.h"
#include "absl/flags/parse.h"

ABSL_FLAG(double, visible_queue_percentage, 0.8, "The percentage of an iceberg order that goest to visible queue");
ABSL_FLAG(double, replenish, 0.5, "The percentage of the hidden porton that goes to visible portion");
ABSL_FLAG(int32_t, hidden_queue_quantity_minimum, 10, "The minimum quantity required in hidden queue, if less than this minimum, all quantity will be transfered to visible queue");

namespace fep::srcs::order
{

  using ::fep::lib::Price4;
  using ::fep::srcs::order::OrderType;
  using ::fep::srcs::stock::Symbol;

  bool OrderPool::AddOrder(std::unique_ptr<Order> order)
  {
    const int32_t order_id = order->order_id();
    if (id_to_order_map_.find(order_id) != id_to_order_map_.end())
    {
      return false;
    }

    int64_t visible_quantity = order->quantity();
    if (order->order_type() == OrderType::ICEBERG)
    {
      visible_quantity *= absl::GetFlag(FLAGS_visible_queue_percentage);
    }
    symbol_to_price_to_visible_quantity_[order->symbol()][order->price()] += visible_quantity;
    id_to_visible_quantity_[order->order_id()] = visible_quantity;

    id_to_order_map_[order_id].swap(order);
    return true;
  }

  bool OrderPool::RemoveOrder(const int64_t order_id)
  {
    const auto kv = id_to_order_map_.find(order_id);
    if (kv == id_to_order_map_.end())
    {
      return false;
    }
    const auto &order = *kv->second;
    symbol_to_price_to_visible_quantity_[order.symbol()][order.price()] -= id_to_visible_quantity_[order_id];
    id_to_visible_quantity_.erase(order_id);
    id_to_order_map_.erase(order_id);
    return true;
  }

  const Order *OrderPool::GetOrder(const int64_t order_id)
  {
    const auto kv = id_to_order_map_.find(order_id);
    if (kv == id_to_order_map_.end())
    {
      return nullptr;
    }
    return kv->second.get();
  }

  bool OrderPool::ModifyOrder(const int64_t order_id, const int64_t quantity_delta, const bool is_visible_queue)
  {
    auto kv = id_to_order_map_.find(order_id);
    if (kv == id_to_order_map_.end())
    {
      return false;
    }
    auto &order = *kv->second;
    const int64_t quantity_after = order.quantity() + quantity_delta;
    order.set_quantity(quantity_after);

    if (is_visible_queue)
    {
      symbol_to_price_to_visible_quantity_[order.symbol()][order.price()] += quantity_delta;
      id_to_visible_quantity_[order_id] += quantity_delta;
    }

    if (!is_visible_queue && id_to_visible_quantity_[order_id] == 0)
    {
      IceberReplenish(order);
    }
    return true;
  }

  int32_t OrderPool::GetQuantityForPrice(const Symbol symbol, const Price4 &price) const
  {
    const auto itr = symbol_to_price_to_visible_quantity_.find(symbol);
    if (itr == symbol_to_price_to_visible_quantity_.end())
    {
      return 0;
    }
    const auto quantity = itr->second.find(price);
    if (quantity == itr->second.end())
    {
      return 0;
    }
    return quantity->second;
  }

  // Add unit tests for this function.
  int32_t OrderPool::GetQuantityInQueue(const int64_t order_id, const bool is_visible_queue) const
  {
    const auto quantity_itr = id_to_order_map_.find(order_id);
    if (quantity_itr == id_to_order_map_.end())
    {
      return 0;
    }
    const auto visible_quantity_itr = id_to_visible_quantity_.find(order_id);
    if (visible_quantity_itr == id_to_visible_quantity_.end())
    {
      return 0;
    }
    return is_visible_queue ? visible_quantity_itr->second : quantity_itr->second->quantity() - visible_quantity_itr->second;
  }

  // Add unit tests for this function.
  void OrderPool::IceberReplenish(const Order &order)
  {
    const int32_t hidden_queue_quantity = GetQuantityInQueue(order.order_id(), /*is_visible_queue*/ false);
    const int32_t hidden_quantity_to_transfer = hidden_queue_quantity <= absl::GetFlag(FLAGS_hidden_queue_quantity_minimum) ? hidden_queue_quantity : hidden_queue_quantity * absl::GetFlag(FLAGS_replenish);

    id_to_visible_quantity_[order.order_id()] = hidden_quantity_to_transfer;
    symbol_to_price_to_visible_quantity_[order.symbol()][order.price()] += hidden_quantity_to_transfer;
  }

} // namespace fep::srcs::order