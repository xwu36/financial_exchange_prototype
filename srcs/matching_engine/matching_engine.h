#ifndef SRCS_MATCHING_ENGINE_MATCHING_ENGINE_H_
#define SRCS_MATCHING_ENGINE_MATCHING_ENGINE_H_

#include <vector>

#include "external/com_google_absl/absl/status/statusor.h"
#include "lib/trade_message.h"
#include "srcs/order/order.h"
#include "srcs/order/order_book.h"
#include "srcs/order/order_pool.h"

namespace fep::srcs::matching_engine
{

  class MatchingEngine
  {
  public:
    MatchingEngine() = default;
    absl::StatusOr<fep::lib::TradeMessage> Process(std::unique_ptr<fep::srcs::order::Order> order);

  private:
    absl::StatusOr<fep::lib::TradeMessage> Sell(std::unique_ptr<fep::srcs::order::Order> order);
    absl::StatusOr<fep::lib::TradeMessage> Buy(std::unique_ptr<fep::srcs::order::Order> order);

    fep::srcs::order::OrderPool order_pool_;
    fep::srcs::order::OrderBook<fep::srcs::order::BidComparator> bid_order_book_;
    fep::srcs::order::OrderBook<fep::srcs::order::AskComparator> ask_order_book_;
  };

} // namespace fep::srcs::matching_engine
#endif