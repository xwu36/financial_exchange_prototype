#include "srcs/order/order.h"

#include <iostream>
#include <fstream>
#include <string>

namespace fep::srcs::order
{

  using ::fep::srcs::stock::SymbolStringToEnum;
  using ::nlohmann::json;

  Order::Order(const json &data)
  {
    const auto &time = data.find("time");
    if (time != data.end())
    {
      time.value().get_to(this->timestamp_sec_);
    }

    const auto &type = data.find("type");
    if (type != data.end())
    {
      if (type.value() == "NEW")
      {
        this->type_ = OrderStatus::NEW;
      }
      else if (type.value() == "CANCEL")
      {
        this->type_ = OrderStatus::CANCEL;
      }
    }

    const auto &order_id = data.find("order_id");
    if (order_id != data.end())
    {
      order_id.value().get_to(this->order_id_);
    }

    const auto &symbol_str = data.find("symbol");
    if (symbol_str != data.end())
    {
      const auto &symbol_enum = SymbolStringToEnum.find(symbol_str.value());
      if (symbol_enum != SymbolStringToEnum.end())
      {
        this->symbol_ = symbol_enum->second;
      }
    }

    const auto &side = data.find("side");
    if (side != data.end())
    {
      if (side.value() == "BUY")
      {
        this->side_ = OrderSide::BUY;
      }
      else if (side.value() == "SELL")
      {
        this->side_ = OrderSide::SELL;
      }
    }

    const auto &quantity = data.find("quantity");
    if (quantity != data.end())
    {
      quantity.value().get_to(this->quantity_);
    }

    const auto &price = data.find("limit_price");
    if (price != data.end())
    {
      this->price_ = fep::lib::Price4(std::string(price.value()));
    }

    this->order_type_ = OrderType::LIMIT;
    const auto &order_type = data.find("order_type");
    if (order_type != data.end())
    {
      if (order_type.value() == "MARKET")
      {
        this->order_type_ = OrderType::MARKET;
      }
    }
  }

} // namespace fep::srcs::order