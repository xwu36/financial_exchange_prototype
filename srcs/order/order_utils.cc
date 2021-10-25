#include "srcs/order/order_utils.h"

#include <iostream>
#include <fstream>
#include <string>

#include "nlohmann/json.hpp"

namespace fep::srcs::order
{
  using ::nlohmann::json;

  std::vector<Order> ReadOrdersFromPath(const std::string &path)
  {
    std::string line;
    std::ifstream infile(path);
    std::vector<Order> orders;
    while (std::getline(infile, line))
    {
      json j = json::parse(line);
      Order order(j);
      orders.emplace_back(order);
    }
    return orders;
  }

} // namespace fep::srcs::order