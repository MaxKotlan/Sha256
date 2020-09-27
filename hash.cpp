#include "hash.h"

Hash::Hash(std::vector<uint8_t> data) : _data(data){
}

std::string Hash::toString(){
    return "";
}

std::vector<uint8_t> Hash::toByteArray(){
    return _data;
}