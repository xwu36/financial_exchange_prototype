#include "srcs/matching_engine/matching_engine.h"

#include "gtest/gtest.h"
#include "srcs/order/order.h"

namespace fep::srcs::matching_engine
{
  namespace
  {
    using ::nlohmann::json;
    using ::fep::srcs::order::Order;

    TEST(MatchingEngineTest, SellOrder)
    {
      MatchingEngine engine;
      json order1_json = {{"order_id", 1}};
      std::unique_ptr<Order> order1 = std::make_unique<Order>(order1_json);
      engine.Sell(std::move(order1));
    }

  }

} // namespace fep::srcs::matching_engine