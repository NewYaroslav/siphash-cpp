#include <gtest/gtest.h>
#include "siphash.hpp"
#include <array>
#include <string>

TEST(SipHash, KnownValues) {
    std::array<char, 16> key = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
    std::string data = "hello";

    EXPECT_EQ(4402678656023170274ULL, siphash_hpp::siphash_2_4(data, key));
    EXPECT_EQ(14986662229302055855ULL, siphash_hpp::siphash_4_8(data, key));
}

TEST(SipHash, PointerKey) {
    const uint8_t key[16] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
    std::string data = "hello";

    siphash_hpp::SipHash h;
    h.init(key, sizeof(key));
    h.update(data);
    EXPECT_EQ(4402678656023170274ULL, h.digest());
}
