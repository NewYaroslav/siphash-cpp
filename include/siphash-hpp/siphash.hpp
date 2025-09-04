#pragma once
#ifndef SIPHASH_HPP_INCLUDED
#define SIPHASH_HPP_INCLUDED

#include <string>
#include <cstdint>

namespace siphash_hpp {

    class SipHash {
    private:
		size_t c, d, index;
		uint64_t v0, v1, v2, v3, m;
		uint8_t input_len;

		template<class T>
		inline const uint64_t read8(const T &p, const size_t o = 0) noexcept {
            return (((uint64_t)((p)[0 + o])) | ((uint64_t)((p)[1 + o]) << 8) |
           ((uint64_t)((p)[2 + o]) << 16) | ((uint64_t)((p)[3 + o]) << 24) |
           ((uint64_t)((p)[4 + o]) << 32) | ((uint64_t)((p)[5 + o]) << 40) |
           ((uint64_t)((p)[6 + o]) << 48) | ((uint64_t)((p)[7 + o]) << 56));
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

        SipHash() {};

        /** \brief Initialize SipHash
         * SipHash-2-4 for best performance
         * SipHash-4-8 for conservative security
         * Siphash-1-3 for performance at the risk of yet-unknown DoS attacks
         * \param key   128-bit secret key
         * \param c     Number of rounds per message block
         * \param d     Number of finalization rounds
         */
        template<class T>
        SipHash(const T &key, const size_t c = 2, const size_t d = 4) {
            init(key, c, d);
        };

        ~SipHash() {};

        /** \brief Initialize SipHash
         * SipHash-2-4 for best performance
         * SipHash-4-8 for conservative security
         * Siphash-1-3 for performance at the risk of yet-unknown DoS attacks
         * \param key   128-bit secret key
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

        SipHash* update(const char byte) noexcept {
            ++input_len;
            m |= (((uint64_t) byte & 0xff) << (index++ * 8));
            if (index >= 8) {
                digest_block();
                index = 0;
                m = 0;
            }
            return this;
        };

        inline SipHash* update(const char* data, const size_t length) noexcept {
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

            return this;
        }

        inline SipHash* update(const std::string &data) noexcept {
            return update(data.data(), data.size());
        }

        const uint64_t digest() noexcept {
            while (index < 7) {
                m |= 0 << (index++ * 8);
            }
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

#endif // SIPHASH_HPP_INCLUDED
