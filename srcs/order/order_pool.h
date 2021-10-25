#ifndef SRCS_ORDER_ORDER_POOL_H_
#define SRCS_ORDER_ORDER_POOL_H_

#include <memory>
#include <unordered_map>

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
      id_to_order_map_[order->order_id()].swap(order);
      return true;
    }

    // Removes an order into the poll.
    // Returns false if the order_id is not present in the pool.
    bool RemoveOrder(const int64_t order_id)
    {
      if (id_to_order_map_.find(order_id) == id_to_order_map_.end())
      {
        return false;
      }
      id_to_order_map_.erase(order_id);
      return true;
    }

    // Returns order detail for a given order_id.
    // Returns a nullptr if the order_id is not present in the pool.
    Order *GetOrder(const int64_t order_id)
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
      return true;
    }

  private:
    std::unordered_map<int64_t, std::unique_ptr<Order>> id_to_order_map_;
  };

} // namespace fep::srcs::order

#endif