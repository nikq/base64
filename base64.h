#pragma once

/*
 * Simple base64 encoder/decoder in C++11
 * copyright(c) 2017 Hajime UCHIMURA (@nikq, nikutama@gmail.com)
 *
 * MIT License
 */

#ifndef __BASE64_H___201708031935
#define __BASE64_H___201708031935

#include <time.h>
#include <stdio.h>
#include <vector>
#include <algorithm>
#include <cassert>

inline std::vector<uint8_t> base64_encode(const std::vector<uint8_t> &bin)
{
    const size_t s = bin.size();
    if (!s)
        return std::vector<uint8_t>();

    auto encode = [](uint8_t uc) {
        return (uc < 26) ? ('A' + uc) : ((uc < 52) ? ('a' + uc - 26) : ((uc < 62) ? ('0' + uc - 52) : ((uc == 62) ? '+' : ((uc == 63) ? '/' : '='))));
    };

    std::vector<uint8_t> result(((s + 2) / 3) * 4);
    for (size_t i = 0, j = 0; i < s; i += 3, j += 4)
    {
        const uint8_t a012345 = (i + 0 < s) ? (bin[i + 0] >> 2) & 0x3F : 0;
        const uint8_t a67____ = (i + 0 < s) ? (bin[i + 0] << 4) & 0x3F : 0;
        const uint8_t b__0123 = (i + 1 < s) ? (bin[i + 1] >> 4) & 0x3F : 0;
        const uint8_t b4567__ = (i + 1 < s) ? (bin[i + 1] << 2) & 0x3F : 0;
        const uint8_t c____01 = (i + 2 < s) ? (bin[i + 2] >> 6) & 0x3F : 0;
        const uint8_t c234567 = (i + 2 < s) ? (bin[i + 2]) & 0x3F : 0;
        result[j + 0] = (i + 0 < s) ? encode(a012345) : '=';
        result[j + 1] = (i + 0 < s) ? encode(a67____ | b__0123) : '=';
        result[j + 2] = (i + 1 < s) ? encode(b4567__ | c____01) : '=';
        result[j + 3] = (i + 2 < s) ? encode(c234567) : '=';
    }
    return result;
}

inline std::vector<uint8_t> base64_decode(const std::vector<uint8_t> &b64)
{
    const size_t s = b64.size();
    if (s == 0)
        return std::vector<uint8_t>();
    assert (s % 4 == 0) ;
    assert (4 <= s) ;
    size_t cntFullBlock = s / 4 - 1 ;

    auto decode = [] (uint8_t uc) {
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
    };
    std::vector<uint8_t>    result ;
    result.reserve (3 * cntFullBlock) ;
    for (size_t i = 0 ; i < cntFullBlock ; ++i) {
        const uint8_t a012345 = decode (b64 [4 * i + 0]);
        const uint8_t a670123 = decode (b64 [4 * i + 1]);
        const uint8_t b456701 = decode (b64 [4 * i + 2]);
        const uint8_t c234567 = decode (b64 [4 * i + 3]);

        result.emplace_back ((a012345 << 2) | (a670123 >> 4)) ;
        result.emplace_back ((a670123 << 4) | (b456701 >> 2)) ;
        result.emplace_back ((b456701 << 6) | (c234567 >> 0)) ;
    }
    // Process the last block
    auto lastIdx = 4 * cntFullBlock ;
    assert (lastIdx < b64.size ()) ;
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

#endif
