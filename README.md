# siphash-hpp

[![Linux](https://github.com/NewYaroslav/siphash-hpp/actions/workflows/CI-Linux.yml/badge.svg)](https://github.com/NewYaroslav/siphash-hpp/actions/workflows/CI-Linux.yml)
[![Win](https://github.com/NewYaroslav/siphash-hpp/actions/workflows/CI-Win.yml/badge.svg)](https://github.com/NewYaroslav/siphash-hpp/actions/workflows/CI-Win.yml)

[SipHash](https://en.wikipedia.org/wiki/SipHash) header only C ++ library

Exapmle:

```cpp
#include "siphash.hpp"

\\...

std::array<char, 16> key = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
std::string data("hello");

std::cout << "SipHash-2-4 for 'hello': " << siphash_hpp::siphash_2_4(data, key) << std::endl;
std::cout << "SipHash-4-8 for 'hello': " << siphash_hpp::siphash_4_8(data, key) << std::endl;
std::cout << "SipHash-4-8 for 'hello': " << siphash_hpp::siphash(data, key, 4, 8) << std::endl;

siphash_hpp::SipHash siphash;
siphash.init(key, 2, 4);
siphash.update(data);
std::cout << "SipHash-2-4 for 'hello': " << siphash.digest() << std::endl;
```






