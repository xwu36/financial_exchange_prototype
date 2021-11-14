#include "srcs/matching_engine/matching_engine_factory.h"

#include <fstream>

#include "absl/flags/flag.h"
#include "nlohmann/json.hpp"

ABSL_FLAG(std::string, tick_size_rule_path, "srcs/matching_engine/data/tick_size_rule.json", "Path which stores tick_size_rule");
ABSL_FLAG(uint32_t, lot_size, 100, "A lot size which shares must be a multiple of ");

namespace fep::srcs::matching_engine
{

  using ::nlohmann::json;

  MatchingEngineFactory::MatchingEngineFactory() : lot_size_(absl::GetFlag(FLAGS_lot_size))
  {
    // Read tick_size rule from a file.
    std::ifstream tick_size_f(absl::GetFlag(FLAGS_tick_size_rule_path));
    json tick_size_j;
    tick_size_f >> tick_size_j;
    tick_size_rule_.FromJson(tick_size_j);
  }

} // namespace fep::srcs::matching_engine