#include "sha256.h"
#include <iostream>
#include <cstdint>
#include <cstring>
#include <algorithm>
#include <climits>

template <typename T>
T swap_endian(T u)
{
    static_assert (CHAR_BIT == 8, "CHAR_BIT != 8");

    union
    {
        T u;
        unsigned char u8[sizeof(T)];
    } source, dest;

    source.u = u;

    for (size_t k = 0; k < sizeof(T); k++)
        dest.u8[k] = source.u8[sizeof(T) - k - 1];

    return dest.u;
}

template<class T>
T ror(T x, unsigned int moves)
{
    return (x >> moves) | (x << sizeof(T) * 8 - moves);
}

typedef std::vector<uint8_t> MessageBlock;

const uint8_t messageBlockByteSize = 64;

void Print(std::vector<uint8_t>& input){
    for (auto e : input)
        std::cout << std::hex << (int)e << " ";
    std::cout << std::endl;
}

uint32_t sigmaZero(uint32_t a) {
    return (ror(ror(a, 7), 18) >> 3);
}

void copyFullMessageBlock(MessageBlock& m, std::vector<uint8_t> &input, uint64_t blockOffset) {
    const uint64_t beginning = blockOffset * messageBlockByteSize;
    const uint64_t end = beginning + messageBlockByteSize;
    std::copy(input.begin()+ beginning, input.begin() + end, m.begin());
}

void copyPartialMessageBlock(MessageBlock& mb, std::vector<uint8_t>& input, uint64_t blockOffset) {
    const uint64_t beginning = blockOffset * messageBlockByteSize;
    const uint64_t end = beginning + (input.size() % messageBlockByteSize);
    std::copy(input.begin() + beginning, input.begin() + end, mb.begin());
    const uint64_t one = 0b10000000;
    uint64_t message_size = swap_endian(input.size());
    std::memcpy(mb.data() + input.size() % messageBlockByteSize, &one, 1);
    std::memcpy(mb.data() + messageBlockByteSize-sizeof(uint64_t), &message_size, sizeof(uint64_t));
}

std::vector<MessageBlock> CreateMessageBlocks(std::vector<uint8_t>& input){
    uint64_t messageBlockCount = (input.size()+9)/64+1;
    std::vector<MessageBlock> messageBlocks(messageBlockCount, MessageBlock(messageBlockByteSize));
    for (uint64_t i = 0; i < messageBlockCount; i++) {

        if ((i + 1) * messageBlockByteSize > input.size()) {
            copyPartialMessageBlock(messageBlocks[i], input, i);
        } else {
            copyFullMessageBlock(messageBlocks[i], input, i);
        }
    }
    return messageBlocks;
}
#include <bitset>

Hash Sha256(std::vector<uint8_t> input){

    std::vector<MessageBlock> messageBlocks = std::move(CreateMessageBlocks(input));

    for (int i = 0; i < messageBlocks.size(); i++) {
        uint32_t w[64];
        for (int j = 0; j < 16; j++) {
            std::memcpy(&w[j], messageBlocks[i].data() + j*sizeof(uint32_t), sizeof(uint32_t));
            uint32_t endianswap = swap_endian(w[j]);
            std::memcpy(&w[j], &endianswap, sizeof(uint32_t));
            std::cout << std::hex << w[j] << std::endl;
        }
        for (int j = 16; j < 64; j++) {
            w[j] = sigmaZero(w[j - 2]) + sigmaZero(w[j - 7]) + w[j - 16];
            uint32_t endianswap = swap_endian(w[j]);
            w[j] = endianswap;
            std::cout << std::bitset<32>(w[j]) << std::endl;
        }
    }

    std::cout << "First Message Block: " << std::endl;
    Print(messageBlocks[0]);
    //Print(input);
    //Pad(input); 
    std::cout << "lol" << std::endl;
    Print(input);

    Hash m(input);
    return m; 
}