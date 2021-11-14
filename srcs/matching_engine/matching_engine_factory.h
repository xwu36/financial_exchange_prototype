#ifndef SRCS_MATCHING_ENGINE_MATCHING_ENGINE_FACTORY_H_
#define SRCS_MATCHING_ENGINE_MATCHING_ENGINE_FACTORY_H_

#include "lib/tick_size_rule.h"
#include "srcs/matching_engine/matching_engine_impl.h"

namespace fep::srcs::matching_engine
{

  class MatchingEngineFactory
  {
  public:
    MatchingEngineFactory();
    fep::srcs::matching_engine::MatchingEngineImpl ProduceMatchingEngine() const
    {
      return fep::srcs::matching_engine::MatchingEngineImpl(tick_size_rule_, lot_size_);
    }

  private:
    fep::lib::TickSizeRule tick_size_rule_;
    uint32_t lot_size_;
  };

} // namespace fep::srcs::matching_engine
#endif