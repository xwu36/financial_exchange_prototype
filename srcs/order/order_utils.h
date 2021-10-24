#ifndef SRCS_ORDER_ORDER_UTILS_H_
#define SRCS_ORDER_ORDER_UTILS_H_

#include <string>
#include <vector>

#include "nlohmann/json.hpp"
#include "srcs/order/order.h"

namespace fep::srcs::order
{

  void FromJson(const nlohmann::json &data, Order &order);

  std::vector<Order> ReadOrdersFromPath(const std::string& path);

} // namespace fep::srcs::order

#endif