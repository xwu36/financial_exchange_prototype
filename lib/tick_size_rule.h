#ifndef LIB_TICK_SIZE_RULE_H_
#define LIB_TICK_SIZE_RULE_H_

#include <vector>

#include "lib/price4.h"
#include "nlohmann/json.hpp"

namespace fep::lib
{
  struct Tick
  {
    fep::lib::Price4 from_price;
    fep::lib::Price4 to_price;
    double tick_size = 0.0;
  };

  class TickSizeRule
  {
  public:
    TickSizeRule() = default;
    bool FromJson(const nlohmann::json &data);
    const std::vector<Tick> &GetTicks() const
    {
      return ticks;
    }

  private:
    std::vector<Tick> ticks;
  };

} // namespace fep::lib

#endif