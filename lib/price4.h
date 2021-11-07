#ifndef LIB_PRICE4_H_
#define LIB_PRICE4_H_

#include <string>

namespace fep::lib
{

  // Represents a price with a fixed constant scale of 4.
  // For example, Price4(1399600) represents $139.96.
  class Price4
  {
  public:
    Price4() = default;
    explicit Price4(long unscaled) : unscaled_(unscaled) {}
    // Converts from string.
    explicit Price4(const std::string &str);
    long unscaled() const { return unscaled_; }
    double scaled() const { return scaled_; }

    bool operator==(const Price4 &that) const
    {
      return this->unscaled() == that.unscaled();
    }
    bool operator!=(const Price4 &that) const
    {
      return this->unscaled() != that.unscaled();
    }
    bool operator<(const Price4 &that) const
    {
      return this->unscaled() < that.unscaled();
    }
    bool operator<=(const Price4 &that) const
    {
      return this->unscaled() <= that.unscaled();
    }
    bool operator>(const Price4 &that) const
    {
      return this->unscaled() > that.unscaled();
    }
    bool operator>=(const Price4 &that) const
    {
      return this->unscaled() >= that.unscaled();
    }

    // Converst to string.
    std::string to_str() const;

  private:
    long unscaled_ = 0;
    double scaled_ = 0;
  };

} // namespace fep::lib

namespace std
{
  template <>
  struct hash<fep::lib::Price4>
  {
    std::size_t operator()(const fep::lib::Price4 &k) const
    {
      // Compute individual hash values for first,
      // second and third and combine them using XOR
      // and bit shifting:

      return std::hash<long>()(k.unscaled());
    }
  };
}

#endif