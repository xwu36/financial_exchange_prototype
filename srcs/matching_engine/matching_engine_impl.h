#ifndef SRCS_MATCHING_ENGINE_MATCHING_ENGINE_IMPL_H_
#define SRCS_MATCHING_ENGINE_MATCHING_ENGINE_IMPL_H_

#include <unordered_map>
#include <vector>

#include "external/com_google_absl/absl/status/statusor.h"
#include "lib/trade_message.h"
#include "srcs/matching_engine/abstract/matching_engine.h"
#include "srcs/order/order.h"
#include "srcs/order/order_book.h"
#include "srcs/order/order_pool.h"
#include "srcs/stock/instrument_symbol.h"

namespace fep::srcs::matching_engine
{

  class MatchingEngineImpl : public MatchingEngine
  {
  public:
    MatchingEngineImpl() = default;
    absl::StatusOr<fep::lib::TradeMessage> Process(std::unique_ptr<fep::srcs::order::Order> order) override;

  private:
    absl::StatusOr<fep::lib::TradeMessage> Sell(std::unique_ptr<fep::srcs::order::Order> order) override;
    absl::StatusOr<fep::lib::TradeMessage> Buy(std::unique_ptr<fep::srcs::order::Order> order) override;
    absl::StatusOr<fep::lib::TradeMessage> Cancel(std::unique_ptr<fep::srcs::order::Order> order) override;

    fep::srcs::order::OrderPool bid_order_pool_;
    fep::srcs::order::OrderPool ask_order_pool_;
    std::unordered_map<fep::srcs::stock::Symbol, fep::srcs::order::OrderBook<fep::srcs::order::BidComparator>> bid_order_books_;
    std::unordered_map<fep::srcs::stock::Symbol, fep::srcs::order::OrderBook<fep::srcs::order::AskComparator>> ask_order_books_;
  };

} // namespace fep::srcs::matching_engine
#endif