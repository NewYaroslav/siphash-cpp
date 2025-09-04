#include <gtest/gtest.h>
#include "siphash.hpp"
#include <array>
#include <string>

struct KeyPtr {
    const char* data;
    size_t len;
    char operator[](size_t i) const { return data[i]; }
    size_t size() const { return len; }
};

TEST(SipHash, KnownValues) {
    std::array<char, 16> key = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
    std::string data = "hello";

    EXPECT_EQ(4402678656023170274ULL, siphash_hpp::siphash_2_4(data, key));
    EXPECT_EQ(14986662229302055855ULL, siphash_hpp::siphash_4_8(data, key));
}

TEST(SipHash, EmptyMessageArrayKey) {
    char key[16];
    for (int i = 0; i < 16; ++i) key[i] = static_cast<char>(i);
    std::string data;
    EXPECT_EQ(8246050544436514353ULL, siphash_hpp::siphash_2_4(data, key));
}

TEST(SipHash, SevenByteMessagePointerKey) {
    char raw_key[16];
    for (int i = 0; i < 16; ++i) raw_key[i] = static_cast<char>(i);
    KeyPtr key{raw_key, sizeof(raw_key)};
    std::string data = {
        char(0x80), char(0x81), char(0x82),
        char(0x83), char(0x84), char(0x85), char(0x86)};
    EXPECT_EQ(3571205124105766914ULL, siphash_hpp::siphash_2_4(data, key));
}

TEST(SipHash, EightByteMessageStdArrayKey) {
    std::array<char, 16> key;
    for (int i = 0; i < 16; ++i) key[i] = static_cast<char>(i);
    std::string data = {
        char(0x80), char(0x81), char(0x82), char(0x83),
        char(0x84), char(0x85), char(0x86), char(0x87)};
    EXPECT_EQ(15639825339957833178ULL, siphash_hpp::siphash_2_4(data, key));
}

TEST(SipHash, LongMessageArrayKey) {
    char key[16];
    for (int i = 0; i < 16; ++i) key[i] = static_cast<char>(i);
    std::string data = {
        char(0x80), char(0x81), char(0x82), char(0x83), char(0x84),
        char(0x85), char(0x86), char(0x87), char(0x88), char(0x89),
        char(0x8A), char(0x8B), char(0x8C), char(0x8D), char(0x8E)};
    EXPECT_EQ(10101490021502548721ULL, siphash_hpp::siphash_2_4(data, key));
}
