#include "srcs/order/order.h"

#include "gtest/gtest.h"

namespace fep::srcs::order
{
  namespace
  {

    using ::fep::lib::Price4;
    using ::fep::lib::Tick;
    using ::fep::lib::TickSizeRule;
    using ::fep::srcs::stock::Symbol;
    using ::nlohmann::json;

    TEST(OrderTest, DefaultConstructor)
    {
      const json data;
      const Order order(data);
      EXPECT_EQ(order.timestamp_sec(), 0);
      EXPECT_EQ(order.type(), OrderStatus::UNKNOWN);
      EXPECT_EQ(order.order_id(), 0);
      EXPECT_EQ(order.symbol(), Symbol::UNKNOWN);
      EXPECT_EQ(order.side(), OrderSide::UNKNOWN);
      EXPECT_EQ(order.quantity(), 0);
      EXPECT_EQ(order.price(), fep::lib::Price4(0));
      EXPECT_EQ(order.order_type(), OrderType::LIMIT);
      json jorder = {
          {"limit_price", "0"}, {"order_id", 0}, {"order_type", "LIMIT"}, {"quantity", 0}, {"symbol", "UNKNOWN"}, {"timestamp_sec", 0}};
      EXPECT_EQ(order.to_json(), jorder);
    }

    TEST(OrderUtilsTest, FromJson)
    {
      const json data = {{"time", 123},
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
      const Order order(data);

      EXPECT_EQ(order.timestamp_sec(), 123);
      EXPECT_EQ(order.type(), OrderStatus::NEW);
      EXPECT_EQ(order.order_id(), 100134);
      EXPECT_EQ(order.symbol(), Symbol::AAPL);
      EXPECT_EQ(order.side(), OrderSide::BUY);
      EXPECT_EQ(order.quantity(), 100);
      EXPECT_EQ(order.price(), fep::lib::Price4(1403000));
      EXPECT_EQ(order.order_type(), OrderType::MARKET);
      EXPECT_EQ(order.time_in_force(), TimeInForce::IOC);
      json jorder = {
          {"limit_price", "140.30"}, {"order_id", 100134}, {"order_type", "MARKET"}, {"quantity", 100}, {"side", "BUY"}, {"symbol", "AAPL"}, {"time_in_force", "IOC"}, {"timestamp_sec", 123}, {"type", "NEW"}};
      EXPECT_EQ(order.to_json(), jorder);
    }

    TEST(OrderUtilsTest, SideFromJson)
    {
      const json data1 = {{"side", "BUY"}};
      const Order order1(data1);
      EXPECT_EQ(order1.side(), OrderSide::BUY);
      json jorder1 = {
          {"limit_price", "0"}, {"order_id", 0}, {"order_type", "LIMIT"}, {"quantity", 0}, {"side", "BUY"}, {"symbol", "UNKNOWN"}, {"timestamp_sec", 0}};
      EXPECT_EQ(order1.to_json(), jorder1);

      const json data2 = {{"side", "SELL"}};
      const Order order2(data2);
      EXPECT_EQ(order2.side(), OrderSide::SELL);
      json jorder2 = {
          {"limit_price", "0"}, {"order_id", 0}, {"order_type", "LIMIT"}, {"quantity", 0}, {"side", "SELL"}, {"symbol", "UNKNOWN"}, {"timestamp_sec", 0}};
      EXPECT_EQ(order2.to_json(), jorder2);
    }

    TEST(OrderUtilsTest, OrderTypeFromJson)
    {
      const json data1 = {{"order_type", "MARKET"}};
      const Order order1(data1);
      EXPECT_EQ(order1.order_type(), OrderType::MARKET);
      json jorder1 = {
          {"limit_price", "0"}, {"order_id", 0}, {"order_type", "MARKET"}, {"quantity", 0}, {"symbol", "UNKNOWN"}, {"time_in_force", "IOC"}, {"timestamp_sec", 0}};
      EXPECT_EQ(order1.to_json(), jorder1);

      const json data2 = {{"order_type", "LIMIT"}};
      const Order order2(data2);
      EXPECT_EQ(order2.order_type(), OrderType::LIMIT);
      json jorder2 = {
          {"limit_price", "0"}, {"order_id", 0}, {"order_type", "LIMIT"}, {"quantity", 0}, {"symbol", "UNKNOWN"}, {"timestamp_sec", 0}};
      EXPECT_EQ(order2.to_json(), jorder2);
    }

    TEST(OrderUtilsTest, OrderStatusFromJson)
    {
      const json data1 = {{"type", "NEW"}};
      const Order order1(data1);
      EXPECT_EQ(order1.type(), OrderStatus::NEW);
      json jorder1 = {
          {"limit_price", "0"}, {"order_id", 0}, {"order_type", "LIMIT"}, {"quantity", 0}, {"symbol", "UNKNOWN"}, {"type", "NEW"}, {"timestamp_sec", 0}};
      EXPECT_EQ(order1.to_json(), jorder1);

      const json data2 = {{"type", "CANCEL"}};
      const Order order2(data2);
      EXPECT_EQ(order2.type(), OrderStatus::CANCEL);
      json jorder2 = {
          {"limit_price", "0"}, {"order_id", 0}, {"order_type", "LIMIT"}, {"quantity", 0}, {"symbol", "UNKNOWN"}, {"type", "CANCEL"}, {"timestamp_sec", 0}};
      EXPECT_EQ(order2.to_json(), jorder2);
    }

    TEST(OrderUtilsTest, TimeInForce)
    {
      const json data1 = {{"time_in_force", "DAY"}};
      const Order order1(data1);
      EXPECT_EQ(order1.time_in_force(), TimeInForce::DAY);
      json jorder1 = {
          {"limit_price", "0"}, {"order_id", 0}, {"order_type", "LIMIT"}, {"quantity", 0}, {"symbol", "UNKNOWN"}, {"time_in_force", "DAY"}, {"timestamp_sec", 0}};
      EXPECT_EQ(order1.to_json(), jorder1);

      const json data2 = {{"time_in_force", "IOC"}};
      const Order order2(data2);
      EXPECT_EQ(order2.time_in_force(), TimeInForce::IOC);
      json jorder2 = {
          {"limit_price", "0"}, {"order_id", 0}, {"order_type", "LIMIT"}, {"quantity", 0}, {"symbol", "UNKNOWN"}, {"time_in_force", "IOC"}, {"timestamp_sec", 0}};
      EXPECT_EQ(order2.to_json(), jorder2);

      const json data3 = {{"time_in_force", "GTC"}};
      const Order order3(data3);
      EXPECT_EQ(order3.time_in_force(), TimeInForce::GTC);
      json jorder3 = {
          {"limit_price", "0"}, {"order_id", 0}, {"order_type", "LIMIT"}, {"quantity", 0}, {"symbol", "UNKNOWN"}, {"time_in_force", "GTC"}, {"timestamp_sec", 0}};
      EXPECT_EQ(order3.to_json(), jorder3);
    }

    TEST(OrderUtilsTest, PriceForGivenTicks)
    {
      const json j = {
          {{"from_price", "0"}, {"to_price", "1"}, {"tick_size", 0.0001}},
          {{"from_price", "1"}, {"tick_size", 0.01}},
      };
      TickSizeRule rule;
      EXPECT_TRUE(rule.FromJson(j));

      const json data1 = {{"time", 123},
                          {"type", "NEW"},
                          {"order_id", 100134},
                          {"symbol", "AAPL"},
                          {"side", "BUY"},
                          {
                              "quantity",
                              100,
                          },
                          {"limit_price", "1.234"}};
      const Order order1(data1);

      EXPECT_EQ(order1.is_valid_order(rule, /*lot_size=*/10), absl::InvalidArgumentError("order price doesn't meet tick_size"));

      const json data2 = {{"time", 123},
                          {"type", "NEW"},
                          {"order_id", 100134},
                          {"symbol", "AAPL"},
                          {"side", "BUY"},
                          {
                              "quantity",
                              100,
                          },
                          {"limit_price", "1.23"},
                          {"order_type", "LIMIT"}};
      const Order order2(data2);

      EXPECT_TRUE(order2.is_valid_order(rule, /*lot_size=*/10).ok());

      const json data3 = {{"time", 123},
                          {"type", "NEW"},
                          {"order_id", 100134},
                          {"symbol", "AAPL"},
                          {"side", "BUY"},
                          {
                              "quantity",
                              100,
                          },
                          {"limit_price", "0.234"},
                          {"order_type", "LIMIT"}};
      const Order order3(data3);

      EXPECT_TRUE(order3.is_valid_order(rule, /*lot_size=*/10).ok());

      const json data4 = {{"time", 123},
                          {"type", "NEW"},
                          {"order_id", 100134},
                          {"symbol", "AAPL"},
                          {"side", "BUY"},
                          {
                              "quantity",
                              100,
                          },
                          {"limit_price", "0.23412"},
                          {"order_type", "LIMIT"}};
      const Order order4(data4);

      EXPECT_EQ(order4.is_valid_order(rule, /*lot_size=*/10), absl::InvalidArgumentError("order price doesn't meet tick_size"));
    }

    TEST(OrderUtilsTest, LotSize)
    {
      json j = {
          {{"from_price", "0"}, {"to_price", "1"}, {"tick_size", 0.0001}},
          {{"from_price", "1"}, {"tick_size", 0.01}},
      };
      TickSizeRule rule;
      EXPECT_TRUE(rule.FromJson(j));

      const json data1 = {{"time", 123},
                          {"type", "NEW"},
                          {"order_id", 100134},
                          {"symbol", "AAPL"},
                          {"side", "BUY"},
                          {
                              "quantity",
                              100,
                          },
                          {"limit_price", "1.23"},
                          {"order_type", "LIMIT"}};
      const Order order1(data1);

      EXPECT_TRUE(order1.is_valid_order(rule, /*lot_size=*/10).ok());

      const json data2 = {{"time", 123},
                          {"type", "NEW"},
                          {"order_id", 100134},
                          {"symbol", "AAPL"},
                          {"side", "BUY"},
                          {
                              "quantity",
                              101,
                          },
                          {"limit_price", "1.23"},
                          {"order_type", "LIMIT"}};
      const Order order2(data2);

      EXPECT_EQ(order2.is_valid_order(rule, /*lot_size=*/10), absl::InvalidArgumentError("order quantity is not multiple of lot_size"));
    }

  } // namespace
} // namespace fep::srcs::order