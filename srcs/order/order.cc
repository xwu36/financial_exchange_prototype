#include "srcs/order/order.h"

#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>

namespace fep::srcs::order
{

  constexpr int kScale4 = 10000;

  using ::fep::lib::Price4;
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
    if (order_type != data.end() && order_type.value() == "MARKET")
    {
      this->order_type_ = OrderType::MARKET;
    }
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
      if (this->price_ <= fep::lib::Price4(0))
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
      const auto &ticks = tick_size_rule.GetTicks();
      const auto tick_itr = std::lower_bound(ticks.cbegin(), ticks.cend(), this->price_,
                                             [](const fep::lib::Tick &lhs, const fep::lib::Price4 &price)
                                             {
                                               return lhs.from_price < price;
                                             }) -
                            1;
      const double tick_size = tick_itr->tick_size;
      const double num_ticks = this->price_.scaled() / tick_size;
      if (num_ticks != (int)num_ticks)
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