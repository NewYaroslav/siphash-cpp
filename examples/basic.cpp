#include <array>
#include <iostream>
#include <string>

#include "siphash.hpp"

// Demonstrates hashing messages with siphash-hpp.
int main() {
    // 16-byte secret key. Replace with a random key in real applications.
    const std::array<char, 16> key{'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
    const std::string msg = "hello";

    // Convenience function for SipHash-2-4.
    const uint64_t hash_2_4 = siphash_hpp::siphash_2_4(msg, key);
    std::cout << "SipHash-2-4(\"" << msg << "\") = " << hash_2_4 << std::endl;

    // Using the SipHash class for custom parameters.
    siphash_hpp::SipHash hasher;
    hasher.init(key, 4, 8); // c = 4, d = 8
    hasher.update(msg);
    const uint64_t hash_4_8 = hasher.digest();
    std::cout << "SipHash-4-8(\"" << msg << "\") = " << hash_4_8 << std::endl;

    return 0;
}
