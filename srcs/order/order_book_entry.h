#ifndef SRCS_ORDER_ORDER_BOOK_ENTRY_H_
#define SRCS_ORDER_ORDER_BOOK_ENTRY_H_

#include <cstdint>

#include "lib/price4.h"

namespace fep::srcs::order
{
  struct OrderBookEntry
  {
    int64_t timestamp_sec = 0;
    int64_t order_id = 0;
    fep::lib::Price4 price;
    bool visible = true;
  };

} // namespace fep::srcs::order

#endif