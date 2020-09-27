class Hash{
    Hash(std::vector<uint8_t> data);
    std::string toString();
    std::vector<uint8_t> toByteArray();
    std::vector<uint8_t> _data;
}