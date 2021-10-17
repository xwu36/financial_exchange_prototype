#ifndef LIB_PRICE4_H_
#define LIB_PRICE4_H_

#include <string>

namespace fep::lib {

// Represents a price with a fixed constant scale of 4.
// For example, Price4(1399600) represents $139.96.
class Price4 {
public:
  Price4() = default;
  explicit Price4(long unscaled) : unscaled_(unscaled) {}
  // Converts from string.
  explicit Price4(const std::string &str);
  long unscaled() const { return unscaled_; }

  bool operator==(const Price4 &that) const {
    return this->unscaled() == that.unscaled();
  }
  bool operator!=(const Price4 &that) const {
    return this->unscaled() != that.unscaled();
  }
  bool operator<(const Price4 &that) const {
    return this->unscaled() < that.unscaled();
  }
  bool operator<=(const Price4 &that) const {
    return this->unscaled() <= that.unscaled();
  }
  bool operator>(const Price4 &that) const {
    return this->unscaled() > that.unscaled();
  }
  bool operator>=(const Price4 &that) const {
    return this->unscaled() >= that.unscaled();
  }

  // Converst to string.
  std::string to_str() const;

private:
  long unscaled_ = 0;
};

} // namespace fep::lib

#endif