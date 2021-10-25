#ifndef SRCS_ORDER_ORDER_UTILS_H_
#define SRCS_ORDER_ORDER_UTILS_H_

#include <string>
#include <vector>

#include "srcs/order/order.h"

namespace fep::srcs::order
{

  std::vector<Order> ReadOrdersFromPath(const std::string &path);

} // namespace fep::srcs::order

#endif