#include "srcs/order/order_utils.h"

#include "lib/price4.h"
#include "gtest/gtest.h"

namespace fep::srcs::order
{
  namespace
  {

    using ::fep::lib::Price4;
    using ::fep::srcs::stock::Symbol;

    TEST(OrderUtilsTest, ReadOrdersFromPath)
    {
      const std::vector<Order> orders = ReadOrdersFromPath("srcs/order/test_data/orders.jsonl");
      EXPECT_EQ(orders.size(), 2);
      EXPECT_EQ(orders[0].order_id(), 100134);
      EXPECT_EQ(orders[0].order_type(), OrderType::LIMIT);
      EXPECT_EQ(orders[0].price(), Price4(1403000));
      EXPECT_EQ(orders[0].quantity(), 100);
      EXPECT_EQ(orders[0].side(), OrderSide::BUY);
      EXPECT_EQ(orders[0].symbol(), Symbol::AAPL);
      EXPECT_EQ(orders[0].timestamp_sec(), 1625787615);
      EXPECT_EQ(orders[0].type(), OrderStatus::NEW);

      EXPECT_EQ(orders[1].order_id(), 100135);
      EXPECT_EQ(orders[1].order_type(), OrderType::MARKET);
      EXPECT_EQ(orders[1].price(), Price4(1423000));
      EXPECT_EQ(orders[1].quantity(), 101);
      EXPECT_EQ(orders[1].side(), OrderSide::SELL);
      EXPECT_EQ(orders[1].symbol(), Symbol::TSLA);
      EXPECT_EQ(orders[1].timestamp_sec(), 1725787615);
      EXPECT_EQ(orders[1].type(), OrderStatus::CANCEL);
    }

  } // namespace
} // namespace fep::srcs::order