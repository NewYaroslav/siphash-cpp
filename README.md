# siphash_cpp

[![Linux](https://github.com/NewYaroslav/siphash-cpp/actions/workflows/CI-Linux.yml/badge.svg)](https://github.com/NewYaroslav/siphash-cpp/actions/workflows/CI-Linux.yml)
[![Win](https://github.com/NewYaroslav/siphash-cpp/actions/workflows/CI-Win.yml/badge.svg)](https://github.com/NewYaroslav/siphash-cpp/actions/workflows/CI-Win.yml)

[SipHash](https://en.wikipedia.org/wiki/SipHash) header only C ++ library.

SipHash is a family of keyed hash functions optimized for short messages. It is widely used to protect hash tables from collision attacks and to authenticate lightweight data.

### Advantages
- Secret key makes hash-flooding attacks impractical
- Small, simple code that runs quickly on short inputs

### Limitations
- Slower than non-cryptographic hashes such as MurmurHash
- 64-bit output is not meant for high-security message authentication or large files

The key must contain at least 16 bytes.

Example:

```cpp
#include "siphash.hpp"

\\...

std::array<char, 16> key = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
std::string data("hello");

std::cout << "SipHash-2-4 for 'hello': " << siphash_cpp::siphash_2_4(data, key) << std::endl;
std::cout << "SipHash-4-8 for 'hello': " << siphash_cpp::siphash_4_8(data, key) << std::endl;
std::cout << "SipHash-4-8 for 'hello': " << siphash_cpp::siphash(data, key, 4, 8) << std::endl;

siphash_cpp::SipHash siphash;
siphash.init(key, 2, 4);
std::cout << "SipHash-2-4 for 'hello': " << siphash.update(data).digest() << std::endl;
```

## Installation

### vcpkg

This library can be installed with [vcpkg](https://github.com/microsoft/vcpkg):

```sh
vcpkg install siphash-cpp
```

After installation, a CMake project may use:

```cmake
find_package(siphash_cpp CONFIG REQUIRED)
target_link_libraries(your_app PRIVATE siphash_cpp::siphash_cpp)
```

To verify the build with vcpkg, run:

```sh
VCPKG_ROOT=/path/to/vcpkg ./scripts/run_tests.sh
```

### Git submodule

```sh
git submodule add https://github.com/NewYaroslav/siphash-cpp.git external/siphash-cpp
```

Then add the subdirectory and link the target in your `CMakeLists.txt`:

```cmake
add_subdirectory(external/siphash-cpp)
target_link_libraries(your_app PRIVATE siphash_cpp::siphash_cpp)
```






