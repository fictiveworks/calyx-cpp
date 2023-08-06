#include "string_converter.hpp"

using namespace calyx;

const std::string 
StdStringConverter::toString(const std::string& stringLike) const
{
    return stringLike;
}


const std::string 
StdStringConverter::fromString(const std::string& stdString) const
{
    return stdString;
}

const std::string 
CStrConverter::toString(const C_Str& stringLike) const
{
    return std::string(stringLike);
}


const C_Str
CStrConverter::fromString(const std::string& stdString) const
{
    char* buffer = new char[stdString.length() + 1];
    std::strncpy(buffer, stdString.c_str(), stdString.size());
    return buffer;
}
