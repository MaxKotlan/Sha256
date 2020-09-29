#include "sha256.h"
#include <iostream>
#include <cstdint>
#include <cstring>

void Print(std::vector<uint8_t>& input){
    for (auto e : input)
        std::cout << std::hex << e << " ";
    std::cout << std::endl;
}

void Pad(std::vector<uint8_t>& input){
    uint64_t length = input.size()*8;
    //uint8_t lengthbytearray[8];
    //uint64_t data[8];
    //std::memcpy(&lengthbytearray, &length, sizeof(uint64_t));
    //std::memcpy(&lengthbytearray, &length, sizeof(uint64_t));

    /*
    if(input.size() < 56)
        input.push_back(0x10000000);
    while (input.size() < 56)
        input.push_back(0);
    for (int i = 0; i < sizeof(uint64_t); i++)
        input.push_back(lengthbytearray[i]);*/
}

Hash Sha256(std::vector<uint8_t> input){
    /*h[n]:= the first 32 bits of the fractional part of the nth prime number*/
    /*  eg: h[0] = sqrt(2)-2*/

    //Print(input);
    //Pad(input); 
    //Print(input);

    input.push_back(0x80);
    while (input.size() < 56)
        input.push_back(0x0);

    uint64_t i[8];
    std::memcpy(i, input.data(), input.size());
    i[7]=input.size();

    /*store in big endian*/
    for (int p = 0; p < 8; p++)
        i[p] = __builtin_bswap64(i[p]);

    for (int k = 0; k < 8; k++)
        std::cout << "0x" << std::hex << k[i] << " ";
    std::cout << std::endl;

    uint32_t h[8] = {
        0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a,
        0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19
    };

    /*32 bit of fractional part of the cubic root of the first 64 prime numbers*/
    uint32_t k[64] = {
        0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
        0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
        0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
        0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
        0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
        0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
        0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
        0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
        0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
        0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
        0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
        0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
        0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
        0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
        0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
        0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
    };

    uint32_t a = h[0];
    uint32_t b = h[1];
    uint32_t c = h[2];
    uint32_t d = h[3];
    uint32_t e = h[4];
    uint32_t f = h[5];
    uint32_t g = h[6];
    uint32_t j = h[7];

    for (int i = 0; i < 64; i++){
        uint32_t S1 = 8;//(e rightrotate 6) xor (e rightrotate 11) xor (e rightrotate 25)
        /*ch := (e and f) xor ((not e) and g)*/
        uint32_t temp1 = 0;
        uint32_t temp2 = 0;
        /*temp1 := h + S1 + ch + k[i] + w[i]
        S0 := (a rightrotate 2) xor (a rightrotate 13) xor (a rightrotate 22)
        maj := (a and b) xor (a and c) xor (b and c)
        temp2 := S0 + maj*/
 
        j = g;
        g = f;
        f = e;
        e = d + temp1;
        d = c;
        c = b;
        b = a;
        a = temp1 + temp2;
    }

    Hash m(input);
    return m; 
}