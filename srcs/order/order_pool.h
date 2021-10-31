#ifndef SRCS_ORDER_ORDER_POOL_H_
#define SRCS_ORDER_ORDER_POOL_H_

#include <memory>
#include <unordered_map>

#include "lib/price4.h"
#include "srcs/order/order.h"

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
      price_to_quantity_[order->price()] += order->quantity();
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
      price_to_quantity_[kv->second->price()] -= kv->second->quantity();
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
      kv->second->set_quantity(kv->second->quantity() + quantity_delta);
      price_to_quantity_[kv->second->price()] += quantity_delta;
      return true;
    }

    int32_t GetQuantityForPrice(const fep::lib::Price4 &price) const
    {
      const auto &kv = price_to_quantity_.find(price);
      if (kv == price_to_quantity_.end())
      {
        return 0;
      }
      return kv->second;
    }

  private:
    std::unordered_map<int64_t, std::unique_ptr<Order>> id_to_order_map_;
    std::unordered_map<fep::lib::Price4, int32_t> price_to_quantity_;
  };

} // namespace fep::srcs::order

#endif