#include "lib/price4.h"

#include <cctype>
#include <string>
#include <system_error>

#include "absl/strings/str_cat.h"
#include "glog/logging.h"

namespace fep::lib {

namespace {

const int kScale4 = 10000;

bool valid_price(const std::string &str) {
  int period_count = 0;
  for (const char c : str) {
    if (std::isdigit(c)) {
      continue;
    }
    if (period_count == 0 && c == '.') {
      ++period_count;
    } else {
      return false;
    }
  }
  return true;
}

} // namespace

Price4::Price4(const std::string &str) {
  unscaled_ = 0;
  if (valid_price(str)) {
    unscaled_ = static_cast<long>(std::stof(str) * kScale4);
  } else {
    LOG(ERROR) << absl::StrCat("Invalid price: ", str);
  }
}

std::string Price4::to_str() const {
  const long first_half = unscaled_ / kScale4;
  const long second_half = unscaled_ / 100 - first_half * 100;
  return absl::StrCat(first_half, ".", second_half);
}

} // namespace fep::lib