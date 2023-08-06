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
