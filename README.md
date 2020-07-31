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
