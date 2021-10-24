#ifndef SRCS_ORDER_ORDER_H_
#define SRCS_ORDER_ORDER_H_

#include <cstdint>
#include <string>

#include "lib/price4.h"
#include "srcs/stock/instrument_symbol.h"

namespace fep::srcs::order
{

  enum class OrderType
  {
    UNKNOWN,
    MARKET,
    LIMIT
  };

  enum class OrderStatus
  {
    UNKNOWN,
    NEW,
    CANCEL
  };

  enum class OrderSide
  {
    UNKNOWN,
    BUY,
    SELL
  };

  struct Order
  {
    int64_t timestamp_sec = 0;
    OrderStatus type = OrderStatus::UNKNOWN;
    int64_t order_id = 0;
    fep::srcs::stock::Symbol symbol = fep::srcs::stock::Symbol::UNKNOWN;
    OrderSide side = OrderSide::UNKNOWN;
    int32_t quantity = 0;
    fep::lib::Price4 price;
    OrderType order_type = OrderType::UNKNOWN;
  };

} // namespace fep::srcs::order

#endif