#ifndef SRCS_ORDER_ORDER_POOL_H_
#define SRCS_ORDER_ORDER_POOL_H_

#include <memory>
#include <unordered_map>

#include "lib/price4.h"
#include "srcs/order/order.h"
#include "srcs/stock/instrument_symbol.h"

namespace fep::srcs::order
{

  class OrderPool
  {
  public:
    // Adds an order into the poll.
    // Returns false if the order_id is already present in the pool.
    bool AddOrder(std::unique_ptr<Order> order)
    {
      if (id_to_order_map_.find(order->order_id()) != id_to_order_map_.end())
      {
        return false;
      }
      symbol_to_price_to_quantity_[order->symbol()][order->price()] += order->quantity();
      id_to_order_map_[order->order_id()].swap(order);
      return true;
    }

    // Removes an order from the poll.
    // Returns false if the order_id is not present in the pool.
    bool RemoveOrder(const int64_t order_id)
    {
      const auto &kv = id_to_order_map_.find(order_id);
      if (kv == id_to_order_map_.end())
      {
        return false;
      }
      auto &order = *kv->second;
      symbol_to_price_to_quantity_[order.symbol()][order.price()] -= order.quantity();
      id_to_order_map_.erase(order_id);
      return true;
    }

    // Returns order detail for a given order_id.
    // Returns a nullptr if the order_id is not present in the pool.
    const Order *GetOrder(const int64_t order_id)
    {
      const auto &kv = id_to_order_map_.find(order_id);
      if (kv == id_to_order_map_.end())
      {
        return nullptr;
      }
      return kv->second.get();
    }

    // Modifies the quantity of a given order.
    bool ModifyOrder(const int64_t order_id, const int64_t quantity_delta)
    {
      auto kv = id_to_order_map_.find(order_id);
      if (kv == id_to_order_map_.end())
      {
        return false;
      }
      auto &order = *kv->second;
      order.set_quantity(order.quantity() + quantity_delta);
      symbol_to_price_to_quantity_[order.symbol()][order.price()] += quantity_delta;
      return true;
    }

    // Returns quantity for a given price and symbol.
    int32_t GetQuantityForPrice(const fep::srcs::stock::Symbol symbol, const fep::lib::Price4 &price) const
    {
      const auto &itr = symbol_to_price_to_quantity_.find(symbol);
      if (itr == symbol_to_price_to_quantity_.end())
      {
        return 0;
      }
      const auto &quantity = itr->second.find(price);
      if (quantity == itr->second.end())
      {
        return 0;
      }
      return quantity->second;
    }

  private:
    std::unordered_map<int64_t, std::unique_ptr<Order>> id_to_order_map_;
    // Stores a price to quantity map for each symbol.
    std::unordered_map<fep::srcs::stock::Symbol, std::unordered_map<fep::lib::Price4, int32_t>> symbol_to_price_to_quantity_;
  };

} // namespace fep::srcs::order

#endif