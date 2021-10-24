#ifndef SRCS_ORDER_ORDER_UTILS_H_
#define SRCS_ORDER_ORDER_UTILS_H_

#include "nlohmann/json.hpp"
#include "srcs/order/order.h"

namespace fep::srcs::order
{

  void FromJson(const nlohmann::json &data, Order &order);

} // namespace fep::srcs::order

#endif