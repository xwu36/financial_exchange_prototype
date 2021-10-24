#include "srcs/order/order_pool.h"

#include <memory>

#include "gtest/gtest.h"

namespace fep::srcs::order
{
  namespace
  {

    TEST(OrderPoolTest, GetOrder)
    {
      std::unique_ptr<Order> order1 = std::unique_ptr<Order>(new Order{.order_id = 1});
      std::unique_ptr<Order> order2 = std::unique_ptr<Order>(new Order{.order_id = 2});
      std::unique_ptr<Order> order3 = std::unique_ptr<Order>(new Order{.order_id = 1});

      OrderPool pool;
      EXPECT_TRUE(pool.AddOrder(std::move(order1)));
      EXPECT_TRUE(pool.AddOrder(std::move(order2)));
      EXPECT_FALSE(pool.AddOrder(std::move(order3)));

      const auto *returned_order1 = pool.GetOrder(/* order_id = */ 1);
      EXPECT_EQ(returned_order1->order_id, 1);
      const auto *returned_order2 = pool.GetOrder(/* order_id = */ 2);
      EXPECT_EQ(returned_order2->order_id, 2);
      EXPECT_EQ(pool.GetOrder(/* order_id = */ 3), nullptr);

      EXPECT_TRUE(pool.RemoveOrder(/* order_id = */ 1));
      EXPECT_FALSE(pool.RemoveOrder(/* order_id= */ 1));
      EXPECT_EQ(pool.GetOrder(/* order_id = */ 1), nullptr);

      pool.ModifyOrder(/* order_id = */ 2, /* quantity_delta = */ 400);
      EXPECT_EQ(pool.GetOrder(/* order_id = */ 2)->quantity, 400);
    }

  } // namespace
} // namespace fep::srcs::order