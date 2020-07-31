# dataunits
C++17 header only library to deal with data units: kilobytes, megabytes, etc.

## Snippet

```cpp
#include <iostream>
#include <cassert>
#include <dataunits/dataunits.hpp>

int main() {
  using namespace dataunits;  
  using namespace dataunits::literals;  

  constexpr auto limit = 2048_MiB;

  auto const maybe_limit = string_to_data<mebibytes>("2048 MiB");
  assert(!!maybe_limit);
  assert(limit == *maybe_limit);  
  assert(to_string(limit) == "2048 MiB");

  // Convert from MiB to GiB
  auto const limit_gb = data_cast<gibibytes>(limit);
  assert(limit_gb() == 2);
  assert(limit_gb.count() == 2);
  
  std::cout << limit_gb << std::endl; // Outputs: 2 GiB
  return 0;
}
```

## Unit List

| Type      | Literal | Value  | Type      | Literal | Value  |
|:----------|--------:|-------:|:----------|--------:|-------:|
| bytes     |    1_B  |      1 |           |         |        |
| kilobytes |    1_kB |   1000 | kibibytes |   1_KiB |   1024 |
| megabytes |    1_MB | 1000^2 | mebibytes |   1_MiB | 1024^2 |
| gigabytes |    1_GB | 1000^3 | gibibytes |   1_GiB | 1024^3 |
| terabytes |    1_TB | 1000^4 | tebibytes |   1_TiB | 1024^4 |
| petabytes |    1_PB | 1000^5 | pebibytes |   1_PiB | 1024^5 |
| exabytes  |    1_EB | 1000^6 | exbibytes |   1_EiB | 1024^6 |
