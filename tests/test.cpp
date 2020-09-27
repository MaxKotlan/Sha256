#include "sha256.h"


void main(){
    Hash result = Sha256(std::vector<uint8_t> input);
    assert(Hash.toString() == "e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855");
}