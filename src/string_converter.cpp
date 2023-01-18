#include "string_converter.hpp"


StdStringConverter::StdStringConverter()
{}

StdStringConverter::~StdStringConverter()
{}

const std::string& 
StdStringConverter::toString(const std::string& stringLike) const
{
    return stringLike;
}


const std::string& 
StdStringConverter::fromString(const std::string& stdString) const
{
    return stdString;
}

const StdStringConverter
StringConverters::STD_STRING_CONVERTER = StdStringConverter();


const StringConverter<CalyxString>& 
StringConverters::DEFAULT_STRING_CONVERTER = STD_STRING_CONVERTER;