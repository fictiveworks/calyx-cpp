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

const std::unique_ptr<StringConverter<String_t>>
StringConverter<String_t>::createDefaultConverter()
{
    return std::make_unique<StdStringConverter>(StdStringConverter());
};