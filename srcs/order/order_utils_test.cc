#include "srcs/order/order_utils.h"

#include "lib/price4.h"
#include "gtest/gtest.h"

namespace fep::srcs::order
{
  namespace
  {

    using ::fep::lib::Price4;
    using ::fep::srcs::stock::Symbol;

    TEST(OrderUtilsTest, DefaultConstructor)
    {
      Order order;
      EXPECT_EQ(order.timestamp_sec, 0);
      EXPECT_EQ(order.type, OrderStatus::UNKNOWN);
      EXPECT_EQ(order.order_id, 0);
      EXPECT_EQ(order.symbol, Symbol::UNKNOWN);
      EXPECT_EQ(order.side, OrderSide::UNKNOWN);
      EXPECT_EQ(order.quantity, 0);
      EXPECT_EQ(order.price, fep::lib::Price4(0));
      EXPECT_EQ(order.order_type, OrderType::UNKNOWN);
    }

    TEST(OrderUtilsTest, FromJson)
    {
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
      EXPECT_EQ(order.symbol, Symbol::AAPL);
      EXPECT_EQ(order.side, OrderSide::BUY);
      EXPECT_EQ(order.quantity, 100);
      EXPECT_EQ(order.price, fep::lib::Price4(1403000));
      EXPECT_EQ(order.order_type, OrderType::MARKET);
    }

    TEST(OrderUtilsTest, BuySideFromJson)
    {
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

      EXPECT_EQ(order.side, OrderSide::BUY);
    }

    TEST(OrderUtilsTest, SellSideFromJson)
    {
      const nlohmann::json data = {{"time", 123},
                                   {"type", "NEW"},
                                   {"order_id", 100134},
                                   {"symbol", "AAPL"},
                                   {"side", "SELL"},
                                   {
                                       "quantity",
                                       100,
                                   },
                                   {"limit_price", "140.30"},
                                   {"order_type", "MARKET"}};
      Order order;
      FromJson(data, order);

      EXPECT_EQ(order.side, OrderSide::SELL);
    }

    TEST(OrderUtilsTest, MarketOrderFromJson)
    {
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

      EXPECT_EQ(order.order_type, OrderType::MARKET);
    }

    TEST(OrderUtilsTest, LimitOrderFromJson)
    {
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
                                   {"order_type", "LIMIT"}};
      Order order;
      FromJson(data, order);

      EXPECT_EQ(order.order_type, OrderType::LIMIT);
    }

    TEST(OrderUtilsTest, NewOrderFromJson)
    {
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
                                   {"order_type", "LIMIT"}};
      Order order;
      FromJson(data, order);

      EXPECT_EQ(order.type, OrderStatus::NEW);
    }

    TEST(OrderUtilsTest, CanceledOrderFromJson)
    {
      const nlohmann::json data = {{"time", 123},
                                   {"type", "CANCEL"},
                                   {"order_id", 100134},
                                   {"symbol", "AAPL"},
                                   {"side", "BUY"},
                                   {
                                       "quantity",
                                       100,
                                   },
                                   {"limit_price", "140.30"},
                                   {"order_type", "LIMIT"}};
      Order order;
      FromJson(data, order);

      EXPECT_EQ(order.type, OrderStatus::CANCEL);
    }

    TEST(OrderUtilsTest, ReadOrdersFromPath) {
      const std::vector<Order> orders = ReadOrdersFromPath("srcs/order/test_data/orders.jsonl");
      EXPECT_EQ(orders.size(), 2);
      EXPECT_EQ(orders[0].order_id, 100134);
      EXPECT_EQ(orders[0].order_type, OrderType::LIMIT);
      EXPECT_EQ(orders[0].price, Price4(1403000));
      EXPECT_EQ(orders[0].quantity, 100);
      EXPECT_EQ(orders[0].side, OrderSide::BUY);
      EXPECT_EQ(orders[0].symbol, Symbol::AAPL);
      EXPECT_EQ(orders[0].timestamp_sec, 1625787615);
      EXPECT_EQ(orders[0].type, OrderStatus::NEW);

      EXPECT_EQ(orders[1].order_id, 100135);
      EXPECT_EQ(orders[1].order_type, OrderType::MARKET);
      EXPECT_EQ(orders[1].price, Price4(1423000));
      EXPECT_EQ(orders[1].quantity, 101);
      EXPECT_EQ(orders[1].side, OrderSide::SELL);
      EXPECT_EQ(orders[1].symbol, Symbol::TSLA);
      EXPECT_EQ(orders[1].timestamp_sec, 1725787615);
      EXPECT_EQ(orders[1].type, OrderStatus::CANCEL);
    }

  } // namespace
} // namespace fep::srcs::order