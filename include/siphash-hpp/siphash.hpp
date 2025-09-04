#pragma once
#ifndef __SIPHASH_HPP_INCLUDED__
#define __SIPHASH_HPP_INCLUDED__

#include <array>
#include <cassert>
#include <cstdint>
#include <cstddef>
#include <string>
#include <tuple>
#include <type_traits>
#include <utility>

#if __cplusplus < 201703L
namespace std {
    template <class...>
    using void_t = void;
}
#endif

namespace siphash_hpp {

    template<typename T, typename = void>
    struct has_static_size : std::false_type {};

    template<typename T>
    struct has_static_size<T, std::void_t<decltype(std::tuple_size<T>::value)>>
            : std::true_type {};

    template<typename T, typename = void>
    struct has_size_method : std::false_type {};

    template<typename T>
    struct has_size_method<T, std::void_t<decltype(std::declval<T>().size())>>
            : std::true_type {};

    template<typename T, bool = has_static_size<T>::value>
    struct static_size_checker;

    template<typename T>
    struct static_size_checker<T, true> {
        static void check() {
            static_assert(sizeof(typename T::value_type) * std::tuple_size<T>::value >= 16,
                          "SipHash key must be at least 16 bytes");
        }
    };

    template<typename T>
    struct static_size_checker<T, false> {
        static void check() {}
    };

    template<typename T, bool = has_size_method<T>::value>
    struct runtime_size_checker;

    template<typename T>
    struct runtime_size_checker<T, true> {
        static void check(const T &key, size_t = 0) {
            assert(key.size() >= 16 && "SipHash key must be at least 16 bytes");
        }
    };

    template<typename T>
    struct runtime_size_checker<T, false> {
        static void check(const T &, size_t = 0) {}
    };

    template<>
    struct runtime_size_checker<const uint8_t*, false> {
        static void check(const uint8_t *, size_t length) {
            assert(length >= 16 && "SipHash key must be at least 16 bytes");
        }
    };

    template<>
    struct runtime_size_checker<const char*, false> {
        static void check(const char *, size_t length) {
            assert(length >= 16 && "SipHash key must be at least 16 bytes");
        }
    };

    class SipHash {
    private:
		size_t c, d, index;
		uint64_t v0, v1, v2, v3, m;
		uint8_t input_len;

        template<class T>
        inline const uint64_t read8(const T &p, const size_t o = 0) noexcept {
            uint64_t result = 0;
            for (size_t i = 0; i < 8; ++i) {
                result |= static_cast<uint64_t>(
                    static_cast<uint8_t>(p[i + o]))
                        << (8 * i);
            }
            return result;
        }

        inline uint64_t rotate_left(
                const uint64_t val,
                const int32_t cnt) noexcept {
            return (val << cnt) | (val >> (64 - cnt));
        }

		inline void compress() noexcept {
            v0 += v1;
            v2 += v3;
            v1 = rotate_left(v1, 13);
            v3 = rotate_left(v3, 16);
            v1 ^= v0;
            v3 ^= v2;
            v0 = rotate_left(v0, 32);
            v2 += v1;
            v0 += v3;
            v1 = rotate_left(v1, 17);
            v3 = rotate_left(v3, 21);
            v1 ^= v2;
            v3 ^= v0;
            v2 = rotate_left(v2, 32);
		}

		inline void digest_block() noexcept {
            v3 ^= m;
            for (size_t i = 0; i < c; ++i) {
                compress();
            }
            v0 ^= m;
		}

    public:

        SipHash() noexcept
            : c(0), d(0), index(0), v0(0), v1(0), v2(0), v3(0), m(0),
              input_len(0) {}

        /** \brief Initialize SipHash
         * SipHash-2-4 for best performance
         * SipHash-4-8 for conservative security
         * Siphash-1-3 for performance at the risk of yet-unknown DoS attacks
         * \param key   128-bit secret key (must be at least 16 bytes)
         * \param c     Number of rounds per message block
         * \param d     Number of finalization rounds
         */
        template<class T, typename std::enable_if<!std::is_pointer<T>::value, int>::type = 0>
        SipHash(const T &key, const size_t c = 2, const size_t d = 4) {
            init(key, c, d);
        };

        SipHash(const char *key, size_t length, const size_t c = 2, const size_t d = 4) {
            init(key, length, c, d);
        };

        SipHash(const uint8_t *key, size_t length, const size_t c = 2, const size_t d = 4) {
            init(key, length, c, d);
        };

        ~SipHash() {};

        /** \brief Initialize SipHash
         * SipHash-2-4 for best performance
         * SipHash-4-8 for conservative security
         * Siphash-1-3 for performance at the risk of yet-unknown DoS attacks
         * \param key   128-bit secret key (must be at least 16 bytes)
         * \param c     Number of rounds per message block
         * \param d     Number of finalization rounds
         */
        template<class T>
        inline void init(
                const T &key,
                const size_t c = 2,
                const size_t d = 4) noexcept {
            this->c = c;
            this->d = d;

            static_assert(!std::is_array<T>::value || sizeof(T) >= 16,
                          "SipHash key must be at least 16 bytes");

            static_size_checker<T>::check();
            runtime_size_checker<T>::check(key, 0);

            const uint64_t k0 = read8(key);
            const uint64_t k1 = read8(key, 8);

            v0 = (0x736f6d6570736575 ^ k0);
            v1 = (0x646f72616e646f6d ^ k1);
            v2 = (0x6c7967656e657261 ^ k0);
            v3 = (0x7465646279746573 ^ k1);

            index = 0;
            input_len = 0;
            m = 0;
        }

        inline void init(
                const char *key,
                size_t length,
                const size_t c = 2,
                const size_t d = 4) noexcept {
            this->c = c;
            this->d = d;

            runtime_size_checker<const char*>::check(key, length);

            const uint64_t k0 = read8(key);
            const uint64_t k1 = read8(key, 8);

            v0 = (0x736f6d6570736575 ^ k0);
            v1 = (0x646f72616e646f6d ^ k1);
            v2 = (0x6c7967656e657261 ^ k0);
            v3 = (0x7465646279746573 ^ k1);

            index = 0;
            input_len = 0;
            m = 0;
        }

        inline void init(
                const uint8_t *key,
                size_t length,
                const size_t c = 2,
                const size_t d = 4) noexcept {
            init(reinterpret_cast<const char*>(key), length, c, d);
        }

        SipHash& update(const char byte) noexcept {
            ++input_len;
            m |= (((uint64_t) byte & 0xff) << (index++ * 8));
            if (index >= 8) {
                digest_block();
                index = 0;
                m = 0;
            }
            return *this;
        };

        inline SipHash& update(const char* data, const size_t length) noexcept {
            size_t i = 0;

            // If there is data left in the internal buffer, fill it first
            if (index) {
                for (; i < length && index < 8; ++i) {
                    ++input_len;
                    m |= (static_cast<uint64_t>(static_cast<uint8_t>(data[i]))
                          << (index++ * 8));
                }
                if (index >= 8) {
                    digest_block();
                    index = 0;
                    m = 0;
                }
            }

            // Process as many 8-byte blocks as possible
            while (i + 8 <= length) {
                input_len += 8;
                m = read8(data, i);
                digest_block();
                i += 8;
                m = 0;
            }

            // Process remaining bytes
            for (; i < length; ++i) {
                ++input_len;
                m |= (static_cast<uint64_t>(static_cast<uint8_t>(data[i]))
                      << (index++ * 8));
            }

            return *this;
        }

        inline SipHash& update(const std::string &data) noexcept {
            return update(data.data(), data.size());
        }

        const uint64_t digest() noexcept {
            while (index < 7) ++index;
            m |= ((uint64_t) input_len) << (index * 8);
            digest_block();
            v2 ^= 0xff;
            for(size_t i = 0; i < d; ++i){
                compress();
            }
            return ((uint64_t) v0 ^ v1 ^ v2 ^ v3);
        }
    }; // SipHash

    template<class T>
    const uint64_t siphash(
            const std::string &data,
            const T &key,
            const size_t c,
            const size_t d) noexcept {
        SipHash h(key, c, d);
        h.update(data);
        return h.digest();
    }

    template<class T>
    const uint64_t siphash_2_4(
            const std::string &data,
            const T &key) noexcept {
        SipHash h(key, 2, 4);
        h.update(data);
        return h.digest();
    }

    template<class T>
    const uint64_t siphash_4_8(
            const std::string &data,
            const T &key) noexcept {
        SipHash h(key, 4, 8);
        h.update(data);
        return h.digest();
    }

    template<class T>
    const uint64_t siphash_1_3(
            const std::string &data,
            const T &key) noexcept {
        SipHash h(key, 1, 3);
        h.update(data);
        return h.digest();
    }
};

#endif // __SIPHASH_HPP_INCLUDED__
