#pragma once

/*
 * Simple base64 encoder/decoder in C++11
 * copyright(c) 2017 Hajime UCHIMURA (@nikq, nikutama@gmail.com)
 *
 * MIT License
 */

#ifndef base64_h__cb0c88b89db746c9a54752a78402846f
#define base64_h__cb0c88b89db746c9a54752a78402846f  1

#include <time.h>
#include <stdio.h>
#include <vector>
#include <algorithm>
#include <cassert>

namespace Base64Internal {
    uint8_t decode (uint8_t uc) {
        if ('A' <= uc && uc <= 'Z') {
            return uc - 'A' ;
        }
        if ('a' <= uc && uc <= 'z') {
            return 26 + uc - 'a' ;
        }
        if ('0' <= uc && uc <= '9') {
            return 52 + uc - '0' ;
        }
        if (uc == '+') {
            return 62 ;
        }
        if (uc == '/') {
            return 63 ;
        }
        assert (false) ;
        return 0 ;
    }
}
/**
 * Encodes supplied bytes into base64 encoded octets.
 */
inline std::vector<uint8_t> base64_encode(const void *input, size_t length)
{
    const uint8_t * bin = static_cast<const uint8_t *> (input) ;
    if (length == 0) {
        return std::vector<uint8_t>();
    }
    auto encode = [](uint8_t uc) {
        return (uc < 26) ? ('A' + uc) : ((uc < 52) ? ('a' + uc - 26) : ((uc < 62) ? ('0' + uc - 52) : ((uc == 62) ? '+' : ((uc == 63) ? '/' : '='))));
    };

    std::vector<uint8_t> result(((length + 2) / 3) * 4);
    for (size_t i = 0, j = 0; i < length; i += 3, j += 4)
    {
        const uint8_t a012345 = (i + 0 < length) ? (bin[i + 0] >> 2) & 0x3F : 0;
        const uint8_t a67____ = (i + 0 < length) ? (bin[i + 0] << 4) & 0x3F : 0;
        const uint8_t b__0123 = (i + 1 < length) ? (bin[i + 1] >> 4) & 0x3F : 0;
        const uint8_t b4567__ = (i + 1 < length) ? (bin[i + 1] << 2) & 0x3F : 0;
        const uint8_t c____01 = (i + 2 < length) ? (bin[i + 2] >> 6) & 0x3F : 0;
        const uint8_t c234567 = (i + 2 < length) ? (bin[i + 2]) & 0x3F : 0;
        result[j + 0] = (i + 0 < length) ? encode(a012345) : '=';
        result[j + 1] = (i + 0 < length) ? encode(a67____ | b__0123) : '=';
        result[j + 2] = (i + 1 < length) ? encode(b4567__ | c____01) : '=';
        result[j + 3] = (i + 2 < length) ? encode(c234567) : '=';
    }
    return result;
}

/**
* Encodes supplied bytes into base64 encoded octets.
*/
inline std::vector<uint8_t> base64_encode (const std::vector<uint8_t> &bin) {
    return base64_encode (bin.data (), bin.size ()) ;
}


/**
 * Decodes supplied base64 encoded octets into raw bytes.
 */
inline std::vector<uint8_t> base64_decode(const void *input, size_t length)
{
    using namespace Base64Internal ;
    const uint8_t * b64 = static_cast<const uint8_t *> (input) ;
    if (length == 0)
        return std::vector<uint8_t>();
    assert (length % 4 == 0) ;
    assert (4 <= length) ;
    size_t cntFullBlock = length / 4 - 1 ;

    std::vector<uint8_t>    result (3 * cntFullBlock) ;
    for (size_t i = 0 ; i < cntFullBlock ; ++i) {
        const uint8_t a012345 = decode (b64 [4 * i + 0]);
        const uint8_t a670123 = decode (b64 [4 * i + 1]);
        const uint8_t b456701 = decode (b64 [4 * i + 2]);
        const uint8_t c234567 = decode (b64 [4 * i + 3]);

        result [3 * i + 0] = (a012345 << 2) | (a670123 >> 4) ;
        result [3 * i + 1] = (a670123 << 4) | (b456701 >> 2) ;
        result [3 * i + 2] = (b456701 << 6) | (c234567 >> 0) ;
    }
    // Process the last input block.
    auto lastIdx = 4 * cntFullBlock ;
    assert (lastIdx < length) ;
    if (b64 [lastIdx + 3] == '=') {
        if (b64 [lastIdx + 2] == '=') {
            // Last block contains 1 octet.
            const uint8_t a012345 = decode (b64 [lastIdx + 0]);
            const uint8_t a670123 = decode (b64 [lastIdx + 1]);
            result.emplace_back ((a012345 << 2) | (a670123 >> 4)) ;
        }
        else {
            // Last block contains 2 octet.
            const uint8_t a012345 = decode (b64 [lastIdx + 0]);
            const uint8_t a670123 = decode (b64 [lastIdx + 1]);
            const uint8_t b456701 = decode (b64 [lastIdx + 2]);
            result.emplace_back ((a012345 << 2) | (a670123 >> 4)) ;
            result.emplace_back ((a670123 << 4) | (b456701 >> 2)) ;
        }
    }
    else {
        // Full block (contains 3 octet).
        const uint8_t a012345 = decode (b64 [lastIdx + 0]);
        const uint8_t a670123 = decode (b64 [lastIdx + 1]);
        const uint8_t b456701 = decode (b64 [lastIdx + 2]);
        const uint8_t c234567 = decode (b64 [lastIdx + 3]);

        result.emplace_back ((a012345 << 2) | (a670123 >> 4)) ;
        result.emplace_back ((a670123 << 4) | (b456701 >> 2)) ;
        result.emplace_back ((b456701 << 6) | (c234567 >> 0)) ;
    }
    return result ;
}

/**
* Decodes supplied base64 encoded octets into raw bytes.
*/
inline std::vector<uint8_t> base64_decode (const std::vector<uint8_t> &input) {
    return base64_decode (input.data (), input.size ()) ;
}

#endif  /* base64_h__cb0c88b89db746c9a54752a78402846f */
