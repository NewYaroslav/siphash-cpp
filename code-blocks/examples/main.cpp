#include <iostream>
#include "siphash.hpp"
#include <cassert>
#include <array>

int main() {
    std::cout << "Hello world!" << std::endl;
    {
        siphash_hpp::SipHash siphash;

        std::array<char, 16> key = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
        siphash.init(key, 2, 4);

        std::string data("hello");
        siphash.update(data);

        const uint64_t expected = 4402678656023170274;
        assert(expected == siphash.digest());
        assert(expected == siphash_hpp::siphash_2_4(data, key));
        std::cout << "SipHash-2-4 for 'hello':          " << siphash_hpp::siphash_2_4(data, key) << std::endl;
        std::cout << "SipHash-2-4 for 'Hello world!':   " << siphash_hpp::siphash_2_4("Hello world!", key) << std::endl;
    }
    {
        siphash_hpp::SipHash siphash;

        std::array<char, 16> key = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
        siphash.init(key, 4, 8);

        std::string data("hello");
        siphash.update(data);

        const uint64_t expected = 14986662229302055855ul;
        assert(expected == siphash.digest());
        assert(expected == siphash_hpp::siphash_4_8(data, key));
        std::cout << "SipHash-4-8 for 'hello':          " << siphash_hpp::siphash_4_8(data, key) << std::endl;
        std::cout << "SipHash-4-8 for 'Hello world!':   " << siphash_hpp::siphash_4_8("Hello world!", key) << std::endl;
    }

    return 0;
}
