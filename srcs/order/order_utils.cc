#include "srcs/order/order_utils.h"

#include <iostream>
#include <fstream>
#include <string>

#include "lib/price4.h"

namespace fep::srcs::order
{

  using ::fep::srcs::stock::SymbolStringToEnum;
  using nlohmann::json;

  void FromJson(const json &data, Order &order)
  {
    data.at("time").get_to(order.timestamp_sec);

    const auto &type = data.at("type");
    if (type == "NEW")
      order.type = OrderStatus::NEW;
    else if (type == "CANCEL")
      order.type = OrderStatus::CANCEL;

    data.at("order_id").get_to(order.order_id);

    const auto &kv = SymbolStringToEnum.find(data.at("symbol"));
    if (kv != SymbolStringToEnum.end())
    {
      order.symbol = kv->second;
    }

    const auto &side = data.at("side");
    if (side == "BUY")
      order.side = OrderSide::BUY;
    else if (side == "SELL")
      order.side = OrderSide::SELL;

    data.at("quantity").get_to(order.quantity);
    const std::string &limit_price = data.at("limit_price");
    order.price = fep::lib::Price4(limit_price);

    order.order_type = OrderType::LIMIT;
    if (data.contains("order_type"))
    {
      const auto &order_type = data.at("order_type");
      if (order_type == "MARKET")
      {
        order.order_type = OrderType::MARKET;
      }
    }
  }

  std::vector<Order> ReadOrdersFromPath(const std::string &path)
  {
    std::string line;
    std::ifstream infile(path);
    std::vector<Order> orders;
    while (std::getline(infile, line))
    {
      json j = json::parse(line);
      Order order;
      FromJson(j, order);
      orders.emplace_back(order);
    }
    return orders;
  }

} // namespace fep::srcs::order