#include "srcs/order/order.h"

#include "srcs/order/order_utils.h"
#include "gtest/gtest.h"

namespace fep::srcs::order {
namespace {

TEST(OrderTest, DefaultConstructor) {
  Order order;
  EXPECT_EQ(order.timestamp_sec, 0);
  EXPECT_EQ(order.type, OrderStatus::UNKNOWN);
  EXPECT_EQ(order.order_id, 0);
  EXPECT_EQ(order.symbol, "");
  EXPECT_EQ(order.side, OrderSide::UNKNOWN);
  EXPECT_EQ(order.quantity, 0);
  EXPECT_EQ(order.price, fep::lib::Price4(0));
  EXPECT_EQ(order.order_type, OrderType::UNKNOWN);
}

TEST(OrderTest, FromJson) {
  const nlohmann::json data = {{"time", 123},
                               {"type", "NEW"},
                               {"order_id", 100134},
                               {"symbol", "AAPL"},
                               {"side", "BUY"},
                               {
                                   "quantity",
                                   100,
                               },
                               {"limit_price", "140.30"},
                               {"order_type", "MARKET"}};
  Order order;
  FromJson(data, order);

  EXPECT_EQ(order.timestamp_sec, 123);
  EXPECT_EQ(order.type, OrderStatus::NEW);
  EXPECT_EQ(order.order_id, 100134);
  EXPECT_EQ(order.symbol, "");
  EXPECT_EQ(order.side, OrderSide::UNKNOWN);
  EXPECT_EQ(order.quantity, 0);
  EXPECT_EQ(order.price, fep::lib::Price4(0));
  EXPECT_EQ(order.order_type, OrderType::UNKNOWN);
}

} // namespace
} // namespace fep::srcs::order