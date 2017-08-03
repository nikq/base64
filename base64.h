#pragma once

/*
 * simple base64 implement in C++11
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

static std::vector<uint8_t> base64_encode(const std::vector<uint8_t> &bin)
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
static std::vector<uint8_t> base64_decode(const std::vector<uint8_t> &b64)
{
    const size_t s = b64.size();
    if (!s)
        return std::vector<uint8_t>();

    int i = s - 1;
    while (b64[i] == '=' && i >= 1)
        i--;
    size_t bin_length = (i + 1) * 3 / 4;
    auto decode = [](uint8_t uc) {
        return ('A' <= uc && uc <= 'Z') ? (uc - 'A') : (('a' <= uc && uc <= 'z') ? (26 + uc - 'a') : (('0' <= uc && uc <= '9') ? (52 + uc - '0') : ((uc == '+') ? 62 : ((uc == '/') ? 63 : 0xFF))));
    };
    std::vector<uint8_t> bin(bin_length);
    for (size_t i = 0, j = 0; i < s; i += 4, j += 3)
    {
        const uint8_t a012345 = decode(b64[i + 0]);
        const uint8_t a670123 = decode(b64[i + 1]);
        const uint8_t b456701 = decode(b64[i + 2]);
        const uint8_t c234567 = decode(b64[i + 3]);
        if (a012345 <= 63 && a670123 <= 63)
            bin[j + 0] = (a012345 << 2) | (a670123 >> 4);
        if (a012345 <= 63 && a670123 <= 63)
            bin[j + 1] = (a670123 << 4) | (b456701 >> 2);
        if (b456701 <= 63 && c234567 <= 63)
            bin[j + 2] = (b456701 << 6) | (c234567 >> 0);
    }
    return bin;
}

#if 0
int main(void)
{
    srand(time(NULL));
    printf("test for 0-65\n");
    /*for (int i = 0; i < 65; i++)
    {
        uint8_t encoded = base64_encode_char(i);
        uint8_t decoded = base64_decode_char(encoded);
        printf("%08x > %c > %08x, %s\n", i, encoded, decoded, i == decoded ? "OK" : "NG");
    }*/
    for (int a = 1; a < 5; a++)
    {
        printf("------- test for some %d-bytes buffer\n", a);
        for (int i = 0; i < 5; i++)
        {
            size_t length = a * (1 + rand() % 5);
            printf("---- length %zu\n", length);
            std::vector<uint8_t> src(length);
            std::for_each(src.begin(), src.end(), [](uint8_t &x) mutable { x = rand(); });
            std::vector<uint8_t> b64 = base64_encode(src);
            std::vector<uint8_t> bin = base64_decode(b64);
            std::for_each(src.begin(), src.end(), [](const uint8_t x) { printf("%02x,", x); });
            printf(" >> ");
            std::for_each(b64.begin(), b64.end(), [](const uint8_t x) { printf("%c", x); });
            printf("\n");
            std::for_each(bin.begin(), bin.end(), [](const uint8_t x) { printf("%02x,", x); });
            printf("\n");
        }
    }
}
#endif
#endif
