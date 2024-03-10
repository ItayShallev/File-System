#include "Helper.h"


std::string intToHex(int integer)
{
    std::stringstream ss;
    ss << std::hex << integer;

    return ss.str();
}
