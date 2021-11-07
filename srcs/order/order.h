#ifndef SRCS_ORDER_ORDER_H_
#define SRCS_ORDER_ORDER_H_

#include <cstdint>
#include <string>

#include "external/com_google_absl/absl/status/status.h"
#include "lib/price4.h"
#include "lib/tick_size_rule.h"
#include "nlohmann/json.hpp"
#include "srcs/stock/instrument_symbol.h"

namespace fep::srcs::order
{

  enum class OrderType
  {
    UNKNOWN,
    MARKET,
    LIMIT
  };

  enum class OrderStatus
  {
    UNKNOWN,
    NEW,
    CANCEL
  };

  enum class OrderSide
  {
    UNKNOWN,
    BUY,
    SELL
  };

  class Order
  {
  public:
    explicit Order(const nlohmann::json &data);

    int64_t timestamp_sec() const
    {
      return timestamp_sec_;
    }
    OrderStatus type() const
    {
      return type_;
    }
    int64_t order_id() const
    {
      return order_id_;
    }
    fep::srcs::stock::Symbol symbol() const
    {
      return symbol_;
    }
    OrderSide side() const
    {
      return side_;
    }
    int32_t quantity() const
    {
      return quantity_;
    }
    fep::lib::Price4 price() const
    {
      return price_;
    }
    OrderType order_type() const
    {
      return order_type_;
    }
    void set_quantity(const int32_t quantity)
    {
      this->quantity_ = quantity;
    }
    absl::Status is_valid_order(const fep::lib::TickSizeRule &tick_size_rule, const int32_t lot_size) const;

  private:
    int64_t timestamp_sec_ = 0;
    OrderStatus type_ = OrderStatus::UNKNOWN;
    int64_t order_id_ = 0;
    fep::srcs::stock::Symbol symbol_ = fep::srcs::stock::Symbol::UNKNOWN;
    OrderSide side_ = OrderSide::UNKNOWN;
    int32_t quantity_ = 0;
    fep::lib::Price4 price_;
    OrderType order_type_ = OrderType::UNKNOWN;
  };

} // namespace fep::srcs::order

#endif