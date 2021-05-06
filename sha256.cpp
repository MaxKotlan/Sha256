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

typedef std::vector<uint8_t> MessageBlock;

const uint8_t messageBlockByteSize = 64;

void Print(std::vector<uint8_t>& input){
    for (auto e : input)
        std::cout << std::hex << (int)e << " ";
    std::cout << std::endl;
}

void copyFullMessageBlock(MessageBlock& m, std::vector<uint8_t> &input, uint64_t blockOffset) {
    std::copy(input.begin(), input.end(), m.begin());
}

void copyPartialMessageBlock(MessageBlock& m, std::vector<uint8_t>& input) {
    std::copy(input.begin(), input.end(), m.begin());
    const uint64_t one = 0b10000000;
    uint64_t message_size = swap_endian(input.size());
    std::memcpy(m.data() + input.size() % messageBlockByteSize, &one, 1);
    std::memcpy(m.data() + messageBlockByteSize-sizeof(uint64_t), &message_size, sizeof(uint64_t));
}

std::vector<MessageBlock> CreateMessageBlocks(std::vector<uint8_t>& input){
    uint64_t messageBlockCount = input.size()/64+1;
    std::vector<MessageBlock> messageBlocks(messageBlockCount, MessageBlock(messageBlockByteSize));
    for (uint64_t i = 0; i < messageBlockCount; i++) {

        if ((i + 1) * messageBlockByteSize > input.size()) {
            copyPartialMessageBlock(messageBlocks[i], input);
        } else {
            copyFullMessageBlock(messageBlocks[i], input, i);
        }
    }
    return messageBlocks;
}

Hash Sha256(std::vector<uint8_t> input){
    std::vector<MessageBlock> messageBlocks = std::move(CreateMessageBlocks(input));
    std::cout << "First Message Block: " << std::endl;
    Print(messageBlocks[0]);
    //Print(input);
    //Pad(input); 
    Print(input);

    Hash m(input);
    return m; 
}