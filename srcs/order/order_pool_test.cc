#include "srcs/order/order_pool.h"

#include <memory>

#include "nlohmann/json.hpp"
#include "gtest/gtest.h"

namespace fep::srcs::order
{
  namespace
  {

    using ::fep::lib::Price4;
    using ::fep::srcs::stock::Symbol;
    using ::nlohmann::json;

    TEST(OrderPoolTest, GetOrder)
    {
      json order1_json = {{"order_id", 1}};
      json order2_json = {{"order_id", 2}};
      json order3_json = {{"order_id", 1}};
      std::unique_ptr<Order> order1 = std::make_unique<Order>(order1_json);
      std::unique_ptr<Order> order2 = std::make_unique<Order>(order2_json);
      std::unique_ptr<Order> order3 = std::make_unique<Order>(order3_json);

      OrderPool pool;
      EXPECT_TRUE(pool.AddOrder(std::move(order1)));
      EXPECT_TRUE(pool.AddOrder(std::move(order2)));
      EXPECT_FALSE(pool.AddOrder(std::move(order3)));

      const auto *returned_order1 = pool.GetOrder(/* order_id = */ 1);
      EXPECT_EQ(returned_order1->order_id(), 1);
      const auto *returned_order2 = pool.GetOrder(/* order_id = */ 2);
      EXPECT_EQ(returned_order2->order_id(), 2);
      EXPECT_EQ(pool.GetOrder(/* order_id = */ 3), nullptr);

      EXPECT_TRUE(pool.RemoveOrder(/* order_id = */ 1));
      EXPECT_FALSE(pool.RemoveOrder(/* order_id= */ 1));
      EXPECT_EQ(pool.GetOrder(/* order_id = */ 1), nullptr);

      pool.ModifyOrder(/* order_id = */ 2, /* quantity_delta = */ 400);
      EXPECT_EQ(pool.GetOrder(/* order_id = */ 2)->quantity(), 400);
    }

    TEST(OrderPoolTest, GetQuantityForPrice)
    {
      json order1_json = {
          {"order_id", 1},
          {"symbol", "AAPL"},
          {
              "quantity",
              10,
          },
          {"limit_price", "1.1"}};
      json order2_json = {
          {"order_id", 2},
          {"symbol", "AAPL"},
          {
              "quantity",
              20,
          },
          {"limit_price", "2.2"}};
      json order3_json = {
          {"order_id", 3},
          {"symbol", "AAPL"},
          {
              "quantity",
              30,
          },
          {"limit_price", "1.1"}};
      std::unique_ptr<Order> order1 = std::make_unique<Order>(order1_json);
      std::unique_ptr<Order> order2 = std::make_unique<Order>(order2_json);
      std::unique_ptr<Order> order3 = std::make_unique<Order>(order3_json);

      OrderPool pool;
      EXPECT_EQ(pool.GetQuantityForPrice(Symbol::AAPL, Price4("1.1")), 0);
      EXPECT_TRUE(pool.AddOrder(std::move(order1)));
      EXPECT_TRUE(pool.AddOrder(std::move(order2)));
      EXPECT_EQ(pool.GetQuantityForPrice(Symbol::AAPL, Price4("1.1")), 10);
      EXPECT_EQ(pool.GetQuantityForPrice(Symbol::AAPL, Price4("2.2")), 20);
      EXPECT_TRUE(pool.AddOrder(std::move(order3)));
      EXPECT_EQ(pool.GetQuantityForPrice(Symbol::AAPL, Price4("1.1")), 40);
      EXPECT_TRUE(pool.RemoveOrder(3));
      EXPECT_EQ(pool.GetQuantityForPrice(Symbol::AAPL, Price4("1.1")), 10);
      EXPECT_TRUE(pool.ModifyOrder(1, -5));
      EXPECT_EQ(pool.GetQuantityForPrice(Symbol::AAPL, Price4("1.1")), 5);
    }
  } // namespace
} // namespace fep::srcs::order