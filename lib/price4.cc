#include "lib/price4.h"

#include <cctype>
#include <string>
#include <system_error>

#include "absl/strings/str_cat.h"
#include "glog/logging.h"

namespace fep::lib
{

  namespace
  {

    constexpr int kScale4 = 10000;

    bool valid_price(const std::string &str)
    {
      int period_count = 0;
      for (const char c : str)
      {
        if (std::isdigit(c))
        {
          continue;
        }
        if (period_count == 0 && c == '.')
        {
          ++period_count;
        }
        else
        {
          return false;
        }
      }
      return true;
    }

  } // namespace

  Price4::Price4(const long unscaled) : unscaled_(unscaled), scaled_(unscaled * 1.0 / kScale4), str_(absl::StrCat(scaled_)) {}

  Price4::Price4(const std::string &str)
  {
    if (valid_price(str))
    {
      scaled_ = std::stod(str);
      unscaled_ = static_cast<long>(scaled_ * kScale4);
      str_ = str;
    }
    else
    {
      LOG(ERROR) << absl::StrCat("Invalid price: ", str);
    }
  }

  std::string Price4::to_str() const
  {
    return str_;
  }

} // namespace fep::lib