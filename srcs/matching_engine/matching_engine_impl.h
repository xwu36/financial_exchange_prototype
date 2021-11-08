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
    MatchingEngineImpl(const fep::lib::TickSizeRule &tick_size_rule, const uint32_t lot_size) : MatchingEngine(tick_size_rule, lot_size) {}
    absl::StatusOr<fep::lib::TradeMessage> Process(std::unique_ptr<fep::srcs::order::Order> order) override;

  private:
    absl::StatusOr<fep::lib::TradeMessage> Sell(std::unique_ptr<fep::srcs::order::Order> order) override;
    absl::StatusOr<fep::lib::TradeMessage> Buy(std::unique_ptr<fep::srcs::order::Order> order) override;
    absl::StatusOr<fep::lib::TradeMessage> Cancel(std::unique_ptr<fep::srcs::order::Order> order) override;
  };

} // namespace fep::srcs::matching_engine
#endif