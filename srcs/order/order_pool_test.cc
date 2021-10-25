#include "srcs/order/order_pool.h"

#include <memory>

#include "nlohmann/json.hpp"
#include "gtest/gtest.h"

namespace fep::srcs::order
{
  namespace
  {

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

  } // namespace
} // namespace fep::srcs::order