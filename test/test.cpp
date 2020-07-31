#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include <dataunits/dataunits.hpp>

using namespace dataunits;
using namespace dataunits::literals;


TEST_CASE("literals") {
  SUBCASE("bytes") {
    auto const s = 9_B;
    REQUIRE(s() == 9);    
  }
  SUBCASE("kibibytes") {
    auto const s = 9_KiB;
    REQUIRE(s() == 9);
  }
  SUBCASE("mebibytes") {
    auto const s = 9_MiB;
    REQUIRE(s() == 9);
  }
  SUBCASE("gibibytes") {
    auto const s = 9_GiB;
    REQUIRE(s() == 9);
  }
  SUBCASE("tebibytes") {
    auto const s = 9_TiB;
    REQUIRE(s() == 9);
  }
  SUBCASE("pebibytes") {
    auto const s = 9_PiB;
    REQUIRE(s() == 9);
  }
  SUBCASE("exbibytes") {
    auto const s = 9_EiB;
    REQUIRE(s() == 9);
  }
  SUBCASE("kilobytes") {
    auto const s = 9_kB;
    REQUIRE(s() == 9);
  }
  SUBCASE("megabytes") {
    auto const s = 9_MB;
    REQUIRE(s() == 9);
  }
  SUBCASE("gigabytes") {
    auto const s = 9_GB;
    REQUIRE(s() == 9);
  }
  SUBCASE("terabytes") {
    auto const s = 9_TB;
    REQUIRE(s() == 9);
  }
  SUBCASE("petabytes") {
    auto const s = 9_PB;
    REQUIRE(s() == 9);
  }
  SUBCASE("exabytes") {
    auto const s = 9_EB;
    REQUIRE(s() == 9);
  }

}


TEST_CASE("to_string") {
  SUBCASE("bytes") {
    auto const s = to_string(11_B);
    REQUIRE(s == "11 B");
  }
  SUBCASE("kibibytes") {
    auto const s = to_string(11_KiB);
    REQUIRE(s == "11 KiB");
  }
  SUBCASE("mebibytes") {
    auto const s = to_string(11_MiB);
    REQUIRE(s == "11 MiB");
  }
  SUBCASE("gibibytes") {
    auto const s = to_string(11_GiB);
    REQUIRE(s == "11 GiB");
  }
  SUBCASE("tebibytes") {
    auto const s = to_string(11_TiB);
    REQUIRE(s == "11 TiB");
  }
  SUBCASE("pebibytes") {
    auto const s = to_string(11_PiB);
    REQUIRE(s == "11 PiB");
  }
  SUBCASE("exbibytes") {
    auto const s = to_string(11_EiB);
    REQUIRE(s == "11 EiB");
  }
  SUBCASE("kilobytes") {
    auto const s = to_string(11_kB);
    REQUIRE(s == "11 kB");
  }
  SUBCASE("megabytes") {
    auto const s = to_string(11_MB);
    REQUIRE(s == "11 MB");
  }
  SUBCASE("gigabytes") {
    auto const s = to_string(11_GB);
    REQUIRE(s == "11 GB");
  }
  SUBCASE("terabytes") {
    auto const s = to_string(11_TB);
    REQUIRE(s == "11 TB");
  }
  SUBCASE("petabytes") {
    auto const s = to_string(11_PB);
    REQUIRE(s == "11 PB");
  }
  SUBCASE("exabytes") {
    auto const s = to_string(11_EB);
    REQUIRE(s == "11 EB");
  }
}


TEST_CASE("data_cast") {
  SUBCASE("B -> KiB") {
    auto const x = 1024_B;
    auto const y = data_cast<kibibytes>(x);
    REQUIRE(y == 1_KiB);
  }
  SUBCASE("MiB -> GiB") {
    auto const x = 2048_MiB;
    auto const y = data_cast<gibibytes>(x);
    REQUIRE(y == 2_GiB);
  }
  SUBCASE("GB -> MiB") {
    auto const x = 1_GB;
    auto const y = data_cast<mebibytes>(x);
    REQUIRE(y == 953_MiB);
  }
}


TEST_CASE("string_to_bytes") {
  SUBCASE("bytes/1") {
    auto const x = string_to_bytes("11");
    REQUIRE(!!x);
    REQUIRE(x->count() == 11);
  }
  SUBCASE("bytes/2") {
    auto const x = string_to_bytes(" 11B");
    REQUIRE(!x);
  }
  SUBCASE("KiB") {
    auto const x = string_to_bytes(" 11 KiB ");
    REQUIRE(!!x);
    REQUIRE(x->count() == 11 * kibi::num);
  }
  SUBCASE("MiB") {
    auto const x = string_to_bytes(" 11 MiB ");
    REQUIRE(!!x);
    REQUIRE(x->count() == 11 * mebi::num);
  }
  SUBCASE("GiB") {
    auto const x = string_to_bytes(" 11 GiB ");
    REQUIRE(!!x);
    REQUIRE(x->count() == 11 * gibi::num);
  }
  SUBCASE("TiB") {
    auto const x = string_to_bytes(" 11 TiB ");
    REQUIRE(!!x);
    REQUIRE(x->count() == 11 * tebi::num);
  }
  SUBCASE("PiB") {
    auto const x = string_to_bytes(" 11 PiB ");
    REQUIRE(!!x);
    REQUIRE(x->count() == 11 * pebi::num);
  }
  SUBCASE("EiB") {
    auto const x = string_to_bytes(" 3 EiB ");
    REQUIRE(!!x);
    REQUIRE(x->count() == 3 * exbi::num);
  }
}
