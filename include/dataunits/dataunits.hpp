/* This file is part of dataunits library
 * Copyright 2020 Andrei Ilin <ortfero@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#pragma once


#include <cstdint>
#include <ratio>
#include <type_traits>
#include <string>
#include <string_view>
#include <optional>


namespace dataunits {
  

  template<class Underlying, class Ratio>
  class data {
  public:
  
    static_assert(std::is_nothrow_default_constructible<Underlying>::value,
      "Underlying type should have `noexcept` default constructor");
    static_assert(std::is_nothrow_copy_constructible<Underlying>::value,
      "Underlying type should have `noexcept` copy constructor");
    static_assert(std::is_nothrow_copy_assignable<Underlying>::value,
      "Underlying type should have `noexcept` assignment operator");
  
    using underlying_type = Underlying;
    using ratio_type = Ratio;
    
    constexpr data() noexcept = default;
    constexpr data(data const&) noexcept = default;
    constexpr data& operator = (data const&) noexcept = default;
    constexpr explicit data(Underlying const& x) noexcept: value_{x} { }
    constexpr Underlying count() const noexcept { return value_; }
    constexpr Underlying operator ()() const noexcept { return value_; }

    constexpr bool operator == (data const& other) const noexcept {
      return value_ == other.value_;
    }

    constexpr bool operator != (data const& other) const noexcept {
      return value_ != other.value_;
    }

    constexpr bool operator > (data const& other) const noexcept {
      return value_ > other.value_;
    }

    constexpr bool operator >= (data const& other) const noexcept {
      return value_ != other.value_;
    }

    constexpr bool operator < (data const& other) const noexcept {
      return value_ < other.value_;
    }

    constexpr bool operator <= (data const& other) const noexcept {
      return value_ <= other.value_;
    }

    constexpr data& operator ++ () noexcept {
      return ++value_;
    }

    constexpr data operator ++ (int) noexcept {
      return capacity(value_++);
    }

    constexpr data& operator -- () noexcept {
      return --value_;
    }

    constexpr data operator -- (int) noexcept {
      return data(value_--);
    }

    constexpr data& operator += (data const& other) noexcept {
      value_ += other.value_;
      return *this;
    }

    constexpr data& operator -= (data const& other) noexcept {
      value_ -= other.value_;
      return *this;
    }

    constexpr data& operator *= (Underlying const& other) noexcept {
      value_ *= other;
      return *this;
    }

    constexpr data& operator /= (Underlying const& other) noexcept {
      value_ /= other;
      return *this;
    }

    constexpr data& operator %= (Underlying const& other) noexcept {
      value_ %= other;
      return *this;
    }
    
  private:
    Underlying value_;
  };


  template<class Underlying, class Ratio>
  constexpr data<Underlying, Ratio> operator + (data<Underlying, Ratio> const& x, data<Underlying, Ratio> const& y) noexcept {
    data z{ x }; z += y;
    return z;
  }


  template<class Underlying, class Ratio>
  constexpr data<Underlying, Ratio> operator - (data<Underlying, Ratio> const& x, data<Underlying, Ratio> const& y) noexcept {
    data z{ x }; z -= y;
    return z;
  }


  template<class Underlying, class Ratio>
  constexpr data<Underlying, Ratio> operator * (data<Underlying, Ratio> const& x, Underlying const& y) noexcept {
    data z{ x }; z *= y;
    return z;
  }


  template<class Underlying, class Ratio>
  constexpr data<Underlying, Ratio> operator * (Underlying const& x, data<Underlying, Ratio> const& y) noexcept {
    data z{ y }; z *= x;
    return z;
  }


  template<class Underlying, class Ratio>
  constexpr data<Underlying, Ratio> operator / (data<Underlying, Ratio> const& x, Underlying const& y) noexcept {
    capacity z{ x }; z /= y;
    return z;
  }


  template<class Underlying, class Ratio>
  constexpr data<Underlying, Ratio> operator % (data<Underlying, Ratio> const& x, Underlying const& y) noexcept {
    data z{ x }; z %= y;
    return z;
  }

  
  template<class Underlying, class Ratio>
  constexpr char const* abbreviation(data<Underlying, Ratio> const&);


  template<class ToUnit, class Underlying, class Ratio>
  constexpr ToUnit data_cast(data<Underlying, Ratio> const& right) {
    using ratio = typename std::ratio_divide<Ratio, typename ToUnit::ratio_type>::type;
    using common_type = typename std::common_type<Underlying, typename ToUnit::underlying_type, std::intmax_t>::type;
    auto const num = common_type(right.count()) * common_type(ratio::num);
    auto const denom = common_type(ratio::den);
    return ToUnit{ num / denom };
  }


  template<class Stream, class Underlying, class Ratio>
  Stream& operator << (Stream& stream, data<Underlying, Ratio> const& x) {
    stream << x() << ' ' << abbreviation(x);
    return stream;
  }
  
  
  template<class Underlying, class Ratio>
  std::string to_string(data<Underlying, Ratio> const& x) {
    auto s{std::to_string(x())};
    s.push_back(' ');
    s.append(abbreviation(x));    
    return s;
  }

  
  using bytes = data<std::int64_t, std::ratio<1>>;

  template<class Underlying>
  constexpr char const* abbreviation(data<Underlying, std::ratio<1>> const&) noexcept {
    return "B";
  }

  namespace literals {
    inline constexpr bytes operator ""_B(unsigned long long x) noexcept {
      return bytes{ static_cast<bytes::underlying_type>(x) };
    }
  }


  using kibi = std::ratio<1024>;
  using kibibytes = data<std::int64_t, kibi>;

  template<class Underlying>
  constexpr char const* abbreviation(data<Underlying, kibi> const&) noexcept {
    return "KiB";
  }
  
  namespace literals {
    inline constexpr kibibytes operator ""_KiB (unsigned long long x) noexcept {
      return kibibytes{static_cast<kibibytes::underlying_type>(x)};
    }
  }


  using mebi = std::ratio<1048576>;
  using mebibytes = data<std::int64_t, mebi>;

  template<class Underlying>
  constexpr char const* abbreviation(data<Underlying, mebi> const&) noexcept {
    return "MiB";
  }

  namespace literals {
    inline constexpr mebibytes operator ""_MiB(unsigned long long x) noexcept {
      return mebibytes{ static_cast<mebibytes::underlying_type>(x) };
    }
  }


  using gibi = std::ratio<1073741824>;
  using gibibytes = data<std::int64_t, gibi>;

  template<class Underlying>
  constexpr char const* abbreviation(data<Underlying, gibi> const&) noexcept {
    return "GiB";
  }

  namespace literals {
    inline constexpr gibibytes operator ""_GiB(unsigned long long x) noexcept {
      return gibibytes{ static_cast<gibibytes::underlying_type>(x) };
    }
  }


  using tebi = std::ratio<1099511627776>;
  using tebibytes = data<std::int64_t, tebi>;

  template<class Underlying>
  constexpr char const* abbreviation(data<Underlying, tebi> const&) noexcept {
    return "TiB";
  }

  namespace literals {
    inline constexpr tebibytes operator ""_TiB(unsigned long long x) noexcept {
      return tebibytes{ static_cast<tebibytes::underlying_type>(x) };
    }
  }


  using pebi = std::ratio<1125899906842624>;
  using pebibytes = data<std::int64_t, std::ratio<1125899906842624>>;

  template<class Underlying>
  constexpr char const* abbreviation(data<Underlying, pebi> const&) noexcept {
    return "PiB";
  }

  namespace literals {
    inline constexpr pebibytes operator ""_PiB(unsigned long long x) noexcept {
      return pebibytes{ static_cast<pebibytes::underlying_type>(x) };
    }
  }


  using exbi = std::ratio<1152921504606846976>;
  using exbibytes = data<std::int64_t, exbi>;
  
  template<class Underlying>
  constexpr char const* abbreviation(data<Underlying, exbi> const&) noexcept {
    return "EiB";
  }

  namespace literals {
    inline constexpr exbibytes operator ""_EiB(unsigned long long x) noexcept {
      return exbibytes{ static_cast<exbibytes::underlying_type>(x) };
    }
  }
  

  using kilobytes = data<std::int64_t, std::kilo>;

  template<class Underlying>
  constexpr char const* abbreviation(data<Underlying, std::kilo> const&) noexcept {
    return "kB";
  }

  namespace literals {
    inline constexpr kilobytes operator ""_kB(unsigned long long x) noexcept {
      return kilobytes{ static_cast<kilobytes::underlying_type>(x) };
    }

    inline constexpr kilobytes operator ""_KB(unsigned long long x) noexcept {
      return kilobytes{ static_cast<kilobytes::underlying_type>(x) };
    }
  }


  using megabytes = data<std::int64_t, std::mega>;

  template<class Underlying>
  constexpr char const* abbreviation(data<Underlying, std::mega> const&) noexcept {
    return "MB";
  }

  namespace literals {
    inline constexpr megabytes operator ""_MB(unsigned long long x) noexcept {
      return megabytes{ static_cast<megabytes::underlying_type>(x) };
    }
  }


  using gigabytes = data<std::int64_t, std::giga>;

  template<class Underlying>
  constexpr char const* abbreviation(data<Underlying, std::giga> const&) noexcept {
    return "GB";
  }

  namespace literals {
    inline constexpr gigabytes operator ""_GB(unsigned long long x) noexcept {
      return gigabytes{ static_cast<gigabytes::underlying_type>(x) };
    }
  }


  using terabytes = data<std::int64_t, std::tera>;

  template<class Underlying>
  constexpr char const* abbreviation(data<Underlying, std::tera> const&) noexcept {
    return "TB";
  }

  namespace literals {
    inline constexpr terabytes operator ""_TB(unsigned long long x) noexcept {
      return terabytes{ static_cast<terabytes::underlying_type>(x) };
    }
  }


  using petabytes = data<std::int64_t, std::peta>;

  template<class Underlying>
  constexpr char const* abbreviation(data<Underlying, std::peta> const&) noexcept {
    return "PB";
  }

  namespace literals {
    inline constexpr petabytes operator ""_PB(unsigned long long x) noexcept {
      return petabytes{ static_cast<petabytes::underlying_type>(x) };
    }
  }


  using exabytes  = data<std::int64_t, std::exa>;
  
  template<class Underlying>
  constexpr char const* abbreviation(data<Underlying, std::exa> const&) noexcept {
    return "EB";
  }

  namespace literals {
    inline constexpr exabytes operator ""_EB(unsigned long long x) noexcept {
      return exabytes{ static_cast<exabytes::underlying_type>(x) };
    }
  }


  namespace detail {

    inline void skip_spaces(char const*& cursor, char const* end) noexcept {
      while(cursor != end)
        switch (*cursor) {
        case ' ': case '\t':
          ++cursor;
          continue;
        default:
          return;
        }
    }


    inline bool parse_unsigned(char const*& cursor, char const* end, bytes::underlying_type& value) noexcept {
      skip_spaces(cursor, end);      
      switch (*cursor) {
      case '0': case '1': case '2': case '3': case '4':
      case '5': case '6': case '7': case '8': case '9':
        break;
      default:
        return false;
      }
      bytes::underlying_type n = 0;
      while (cursor != end)
        switch (*cursor) {
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
          n *= 10;
          n += (*cursor) - char('0');
          if (n < 0)
            return false;
          ++cursor;
          continue;
        default:
          value = n;
          return true;
        }
      value = n;
      return true;
    }


    template<class Decimal, class Binary>
    inline bool parse_unit(char const*& cursor, std::intmax_t& num) {
      ++cursor;
      switch (*cursor) {
      case 'i': case 'I':
        ++cursor;
        switch (*cursor) {
        case 'B': case 'b':
          num = Binary::num;
          ++cursor;
          return true;
        default:
          return false;
        }
        return false;
      case 'B': case 'b':
        num = Decimal::num;
        ++cursor;
        return true;
      default:
        return false;
      }
    }

  } // detail
  
  
  inline std::optional<bytes> view_to_bytes(std::string_view const& sv) noexcept {

    std::optional<bytes> result;
    
    if (sv.empty())
      return result;

    char const* cursor = sv.data();
    char const* end = sv.data() + sv.size();
    bytes::underlying_type value;

    if (!detail::parse_unsigned(cursor, end, value))
      return result;

    if (cursor == end)
      return result = bytes{ value };
    
    switch (*cursor) {
    case ' ': case '\t':
      detail::skip_spaces(cursor, end);
      if (cursor == end)
        return result = bytes{ value };
      break;
    default:
      return result;
    }
    
    std::intmax_t num = 1;

    switch (*cursor) {
    case 'B': case 'b':
      ++cursor;
      break;
    case 'k': case 'K':
      if (!detail::parse_unit<std::kilo, kibi>(cursor, num))
        return result;
      break;
    case 'M': case 'm':
      if (!detail::parse_unit<std::mega, mebi>(cursor, num))
        return result;
      break;
    case 'G': case 'g':
      if (!detail::parse_unit<std::giga, gibi>(cursor, num))
        return result;
      break;
    case 'T': case 't':
      if (!detail::parse_unit<std::tera, tebi>(cursor, num))
        return result;
      break;
    case 'P': case 'p':
      if (!detail::parse_unit<std::peta, pebi>(cursor, num))
        return result;
      break;
    case 'E': case 'e':
      if (!detail::parse_unit<std::exa, exbi>(cursor, num))
        return result;
      break;
    default:
      return result;
    }

    value *= num;
    if (value < 0)
      return result;

    if(cursor == end)
      return result = bytes{ value };

    detail::skip_spaces(cursor, end);

    if (cursor != end)
      return result;

    return result = bytes{ value };
  }


  inline std::optional<bytes> string_to_bytes(std::string s) noexcept {
    return view_to_bytes(std::string_view{ s.data(), s.size() });
  }
  
  
  template<typename T>
  std::optional<T> view_to_data(std::string_view const& view) noexcept {
    std::optional<T> result;
    auto const maybe_bytes = view_to_bytes(view);
    if(!maybe_bytes)
      return result;
    return result = data_cast<T>(*maybe_bytes);
  }

  
  template<typename T>  
  std::optional<T> string_to_data(std::string s) noexcept {
    return view_to_data<T>(std::string_view{ s.data(), s.size() });
  }   
}
