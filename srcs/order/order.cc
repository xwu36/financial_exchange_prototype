#include "srcs/order/order.h"

#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>

#include "lib/json_parser.h"

namespace fep::srcs::order
{
  constexpr int kScale4 = 10000;
  constexpr double kPriceError = 1e-6;

  using ::fep::lib::GetValueForKey;
  using ::fep::lib::Price4;
  using ::fep::srcs::stock::SymbolEnumToString;
  using ::fep::srcs::stock::SymbolStringToEnum;
  using ::nlohmann::json;

  Order::Order(const json &data)
  {
    this->timestamp_sec_ = GetValueForKey<int64_t>(data, "time", /*default_value=*/0);
    this->order_id_ = GetValueForKey<int64_t>(data, "order_id", /*default_value=*/0);
    this->quantity_ = GetValueForKey<int32_t>(data, "quantity", /*default_value=*/0);
    this->price_ = Price4(GetValueForKey<std::string>(data, "limit_price", /*default_value=*/"0"));

    const std::string type = GetValueForKey<std::string>(data, "type", /*default_value=*/"");
    if (type == "NEW")
    {
      this->type_ = OrderStatus::NEW;
    }
    else if (type == "CANCEL")
    {
      this->type_ = OrderStatus::CANCEL;
    }

    const std::string symbol = GetValueForKey<std::string>(data, "symbol", /*default_value=*/"");
    const auto symbol_enum = SymbolStringToEnum.find(symbol);
    if (symbol_enum != SymbolStringToEnum.end())
    {
      this->symbol_ = symbol_enum->second;
    }

    const std::string side = GetValueForKey<std::string>(data, "side", /*default_value=*/"");
    if (side == "BUY")
    {
      this->side_ = OrderSide::BUY;
    }
    else if (side == "SELL")
    {
      this->side_ = OrderSide::SELL;
    }

    this->order_type_ = OrderType::LIMIT;
    const std::string order_type = GetValueForKey<std::string>(data, "order_type", /*default_value=*/"");
    if (order_type == "MARKET")
    {
      this->order_type_ = OrderType::MARKET;
    }

    const std::string time_in_force = GetValueForKey<std::string>(data, "time_in_force", /*default_value=*/"");
    if (time_in_force == "DAY")
    {
      this->time_in_force_ = TimeInForce::DAY;
    }
    else if (time_in_force == "IOC" || order_type == "MARKET")
    {
      this->time_in_force_ = TimeInForce::IOC;
    }
    else if (time_in_force == "GTC")
    {
      this->time_in_force_ = TimeInForce::GTC;
    }
  }

  json Order::to_json() const
  {
    json j;
    j["timestamp_sec"] = this->timestamp_sec_;
    switch (this->type_)
    {
    case OrderStatus::NEW:
      j["type"] = "NEW";
      break;
    case OrderStatus::CANCEL:
      j["type"] = "CANCEL";
      break;
    default:
      break;
    }
    j["order_id"] = this->order_id_;
    const auto symbol = SymbolEnumToString.find(this->symbol_);
    if (symbol != SymbolEnumToString.end())
    {
      j["symbol"] = symbol->second;
    }
    switch (this->side_)
    {
    case OrderSide::BUY:
      j["side"] = "BUY";
      break;
    case OrderSide::SELL:
      j["side"] = "SELL";
      break;
    default:
      break;
    }
    j["quantity"] = this->quantity_;
    j["limited_price"] = this->price_.to_str();
    switch (this->order_type_)
    {
    case OrderType::MARKET:
      j["order_type"] = "MARKET";
      break;
    case OrderType::LIMIT:
      j["order_type"] = "LIMIT";
      break;
    default:
      break;
    }
    switch (this->time_in_force_)
    {
    case TimeInForce::DAY:
      j["time_in_force"] = "DAY";
      break;
    case TimeInForce::IOC:
      j["time_in_force"] = "IOC";
      break;
    case TimeInForce::GTC:
      j["time_in_force"] = "GTC";
      break;
    default:
      break;
    }
    return j;
  }

  // TODO(): Add unit tests for this function.
  absl::Status Order::is_valid_order(const fep::lib::TickSizeRule &tick_size_rule, const int32_t lot_size) const
  {
    if (this->timestamp_sec_ <= 0)
    {
      return absl::InvalidArgumentError("order has a bad timestamp");
    }
    if (this->order_id_ <= 0)
    {
      return absl::InvalidArgumentError("order has a bad order id");
    }

    if (this->type_ == OrderStatus::NEW)
    {
      if (this->symbol_ == fep::srcs::stock::Symbol::UNKNOWN)
      {
        return absl::InvalidArgumentError("order has unknown symbol");
      }
      if (this->side_ == OrderSide::UNKNOWN)
      {
        return absl::InvalidArgumentError("order has unknown side");
      }
      if (this->quantity_ <= 0)
      {
        return absl::InvalidArgumentError("order quantity is less than 0");
      }
      if (this->order_type_ == OrderType::LIMIT && this->price_ <= fep::lib::Price4(0))
      {
        return absl::InvalidArgumentError("order price is less than 0");
      }
      if (this->order_type_ == OrderType::UNKNOWN)
      {
        return absl::InvalidArgumentError("order has unknown type");
      }
      if (this->quantity_ % lot_size != 0)
      {
        return absl::InvalidArgumentError("order quantity is not multiple of lot_size");
      }
      const auto ticks = tick_size_rule.GetTicks();
      const auto tick_itr = std::lower_bound(ticks.cbegin(), ticks.cend(), this->price_,
                                             [](const fep::lib::Tick &lhs, const fep::lib::Price4 &price)
                                             {
                                               return lhs.from_price < price;
                                             }) -
                            1;
      const double tick_size = tick_itr->tick_size;
      const double num_ticks = this->price_.scaled() / tick_size;
      if (std::abs(num_ticks - std::round(num_ticks)) > kPriceError)
      {
        return absl::InvalidArgumentError("order price doesn't meet tick_size");
      }
      return absl::OkStatus();
    }

    if (this->type_ == OrderStatus::CANCEL)
    {
      return absl::OkStatus();
    }

    return absl::InvalidArgumentError("order type unknown");
  }

} // namespace fep::srcs::order