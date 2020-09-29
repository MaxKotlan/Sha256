#include "sha256.h"
#include <cassert>
#include <iostream>

int main(){
    std::vector<uint8_t> input = {'a', 'b', 'c', '\0'};
    Hash result = Sha256(input);
    std::cout << "Result: " << result.toString() << std::endl;
    assert(result.toString() == "e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855");
}