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
    bool AddOrder(std::unique_ptr<Order> order);

    // Removes an order from the poll.
    // Returns false if the order_id is not present in the pool.
    bool RemoveOrder(const int64_t order_id);

    // Returns order detail for a given order_id.
    // Returns a nullptr if the order_id is not present in the pool.
    const Order *GetOrder(const int64_t order_id);

    // Modifies the quantity of a given order.
    bool ModifyOrder(const int64_t order_id, const int64_t quantity_delta);

    // Returns quantity for a given price and symbol.
    // Returns 0 if there is no match.
    int32_t GetQuantityForPrice(const fep::srcs::stock::Symbol symbol, const fep::lib::Price4 &price) const;

  private:
    std::unordered_map<int64_t, std::unique_ptr<Order>> id_to_order_map_;
    // Stores a price to quantity map for each symbol.
    std::unordered_map<fep::srcs::stock::Symbol, std::unordered_map<fep::lib::Price4, int32_t>> symbol_to_price_to_quantity_;
  };

} // namespace fep::srcs::order

#endif