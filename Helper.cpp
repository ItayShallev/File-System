#include "Helper.h"


std::string intToHex(int integer)
{
    std::stringstream ss;
    ss << std::hex << integer;

    return ss.str();
}


std::vector<std::string> splitEntry(const std::string& entry)
{
    std::vector<std::string> tokens;
    std::istringstream ss(entry);
    std::string token;

    while (std::getline(ss, token, ENTRY_DELIMITER)) {
        tokens.push_back(token);
    }

    return tokens;
}
