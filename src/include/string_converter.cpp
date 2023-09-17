
#include <cstring>

#include "string_converter.h"

using namespace calyx;

std::string 
StdStringConverter::toString(const std::string& stringLike) const
{
    return stringLike;
}


std::string 
StdStringConverter::fromString(const std::string& stdString) const
{
    return stdString;
}

std::string 
CStrConverter::toString(const C_Str& stringLike) const
{
    return {stringLike};
}


C_Str 
CStrConverter::fromString(const std::string& stdString) const
{
    auto buffer = new char[stdString.length() + 1];
    std::strncpy(buffer, stdString.c_str(), stdString.size());
    return buffer;
}
