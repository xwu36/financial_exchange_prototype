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
    bool RemoveOrder(int64_t order_id);

    // Returns order detail for a given order_id.
    // Returns a nullptr if the order_id is not present in the pool.
    const Order *GetOrder(int64_t order_id);

    // Modifies the quantity for a given order.
    bool ModifyOrder(int64_t order_id, int64_t quantity_delta, const bool is_visible_queue = true);

    // Returns visible quantity for a given price and symbol.
    // Returns 0 if there is no match.
    int32_t GetVisibleQuantityForPrice(fep::srcs::stock::Symbol symbol, const fep::lib::Price4 &price) const;

    // Returns offer quantity in visible or hidden queue.
    int32_t GetQuantityInQueue(int64_t order_id, bool is_visible_queue) const;

    void IceberReplenish(const fep::srcs::order::Order &order_id);

    // Writes unmatched portion of GTC orders to a given path.
    // Returns false if fail to open the given path.
    bool OutputGtcOrders(const std::string &path);

    // Clear this pool.
    void ClearPool();

  private:
    std::unordered_map<int64_t, std::unique_ptr<Order>> id_to_order_map_;
    // Stores a price to quantity map for each symbol.
    std::unordered_map<fep::srcs::stock::Symbol, std::unordered_map<fep::lib::Price4, int32_t>> symbol_to_price_to_visible_quantity_;
    std::unordered_map<int64_t, int64_t> id_to_visible_quantity_;
  };

} // namespace fep::srcs::order

#endif