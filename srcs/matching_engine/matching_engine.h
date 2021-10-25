#ifndef SRCS_MATCHING_ENGINE_MATCHING_ENGINE_H_
#define SRCS_MATCHING_ENGINE_MATCHING_ENGINE_H_

#include <vector>

#include "srcs/order/order.h"
#include "srcs/order/order_book.h"
#include "srcs/order/order_pool.h"

namespace fep::srcs::matching_engine
{

  class MatchingEngine
  {
  public:
    MatchingEngine() = default;

    void Sell(std::unique_ptr<fep::srcs::order::Order> order);

  private:
    fep::srcs::order::OrderPool order_pool_;
    fep::srcs::order::OrderBook<fep::srcs::order::BidComparator> bid_order_book_;
    fep::srcs::order::OrderBook<fep::srcs::order::AskComparator> ask_order_book_;
  };

} // namespace fep::srcs::matching_engine
#endif