#ifndef SRCS_ORDER_ORDER_UTILS_H_
#define SRCS_ORDER_ORDER_UTILS_H_

#include <cstdint>

#include "lib/price4.h"
#include "nlohmann/json.hpp"
#include "srcs/stock/instrument_symbol.h"

namespace fep::srcs::order {

void FromJson(const nlohmann::json &data, Order &order) {
  data.at("time").get_to(order.timestamp_sec);

  const auto &type = data.at("type");
  if (type == "NEW")
    order.type = OrderStatus::NEW;
  else if (type == "CANCEL")
    order.type = OrderStatus::CANCEL;

  data.at("order_id").get_to(order.order_id);
  data.at("symbol").get_to(order.symbol);

  const auto &side = data.at("side");
  if (side == "BUY")
    order.side = OrderSide::BUY;
  else if (side == "SELL")
    order.side = OrderSide::SELL;

  data.at("quantity").get_to(order.quantity);
  order.price = fep::lib::Price4(0);

  const auto &order_type = data.at("order_type");
  if (order_type == "MARKET")
    order.order_type = OrderType::MARKET;
  else if (type == "LIMIT")
    order.order_type = OrderType::LIMIT;
};

} // namespace fep::srcs::order

#endif