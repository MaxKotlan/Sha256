#include <vector>
#include <string>

class Hash{
    public:
    Hash(std::vector<uint8_t> data);
    std::string toString();
    std::vector<uint8_t> toByteArray();
    private:
    std::vector<uint8_t> _data;
};