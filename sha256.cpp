#include "sha256.h"
#include <iostream>
#include <cstdint>
#include <cstring>
#include <algorithm>

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

void ChangeMemEndianness(uint64_t* mem, size_t size)
{
    uint64_t m1 = 0xFF00FF00FF00FF00ULL, m2 = m1 >> CHAR_BIT;

    size = (size + (sizeof(uint64_t) - 1)) / sizeof(uint64_t);
    for (; size; size--, mem++)
        *mem = ((*mem & m1) >> CHAR_BIT) | ((*mem & m2) << CHAR_BIT);
}

void copyPartialMessageBlock(MessageBlock& m, std::vector<uint8_t>& input) {
    std::copy(input.begin(), input.end(), m.begin());
    const uint64_t one = 0b10000000;
    uint64_t message_size = input.size();
    ChangeMemEndianness(&message_size, sizeof(uint64_t));
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