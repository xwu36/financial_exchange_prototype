#include "srcs/order/order_pool.h"

namespace fep::srcs::order
{

  using ::fep::lib::Price4;
  using ::fep::srcs::stock::Symbol;

  bool OrderPool::AddOrder(std::unique_ptr<Order> order)
  {
    const int32_t order_id = order->order_id();
    if (id_to_order_map_.find(order_id) != id_to_order_map_.end())
    {
      return false;
    }
    symbol_to_price_to_quantity_[order->symbol()][order->price()] += order->quantity();
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
    symbol_to_price_to_quantity_[order.symbol()][order.price()] -= order.quantity();
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

  bool OrderPool::ModifyOrder(const int64_t order_id, const int64_t quantity_delta)
  {
    auto kv = id_to_order_map_.find(order_id);
    if (kv == id_to_order_map_.end())
    {
      return false;
    }
    auto &order = *kv->second;
    const int64_t quantity_after = order.quantity() + quantity_delta;
    if (quantity_after < 0)
    {
      return false;
    }
    order.set_quantity(quantity_after);
    symbol_to_price_to_quantity_[order.symbol()][order.price()] += quantity_delta;
    return true;
  }

  int32_t OrderPool::GetQuantityForPrice(const Symbol symbol, const Price4 &price) const
  {
    const auto itr = symbol_to_price_to_quantity_.find(symbol);
    if (itr == symbol_to_price_to_quantity_.end())
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

} // namespace fep::srcs::order