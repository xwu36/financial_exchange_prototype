#ifndef SRCS_MATCHING_ENGINE_MATCHING_ENGINE_FACTORY_H_
#define SRCS_MATCHING_ENGINE_MATCHING_ENGINE_FACTORY_H_

#include "srcs/matching_engine/matching_engine_impl.h"

namespace fep::srcs::matching_engine
{

  class MatchingEngineFactory
  {
  public:
    MatchingEngineFactory() = default;
    fep::srcs::matching_engine::MatchingEngineImpl ProduceMatchingEngine() const
    {
      return fep::srcs::matching_engine::MatchingEngineImpl();
    }
  };

} // namespace fep::srcs::matching_engine
#endif