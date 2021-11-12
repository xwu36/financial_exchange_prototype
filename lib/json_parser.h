#ifndef LIB_JSON_PARSER_H_
#define LIB_JSON_PARSER_H_

#include "absl/strings/string_view.h"
#include "nlohmann/json.hpp"

namespace fep::lib
{
  template <class T>
  T GetValueForKey(const nlohmann::json &data, absl::string_view key, const T &default_value)
  {
    T res = default_value;
    const auto itr = data.find(key);
    if (itr == data.end())
    {
      return res;
    }
    return itr->get<T>();
  }

} // namespace fep::lib

#endif