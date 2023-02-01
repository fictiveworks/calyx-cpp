#include "errors.hpp"
#include <string>
#include <sstream>

using namespace calyx;

const std::function<String_t(const String_t &, const StringConverter<String_t> &)>
    Errors::undefinedRule = [&](const String_t &symbol, const StringConverter<String_t> &converter) -> String_t
{
    std::ostringstream stream;
    stream << "Uundefined rule: '" << converter.toString(symbol) << "'";
    return converter.fromString(stream.str());
};

const std::function<String_t(const String_t &, const StringConverter<String_t> &)>
    Errors::undefinedFilter = [&](const String_t &symbol, const StringConverter<String_t> &converter) -> String_t
{
    std::ostringstream stream;
    stream << "Undefined filter: '" << converter.toString(symbol) << "'";
    return converter.fromString(stream.str());
};

const std::function<String_t(const String_t &, const StringConverter<String_t> &)>
    Errors::incorrectFilterSignature = [&](const String_t &symbol, const StringConverter<String_t> &converter) -> String_t
{
    std::ostringstream stream;
    stream << "Incorrect method signature for filter: " << converter.toString(symbol) << "'";
    return converter.fromString(stream.str());
};

Error::Error(): 
    _message(StringConverters::CALYX_STRING_CONVERTER.fromString("")),
    _hasError(false)
{
}

bool Error::hasError() const
{
    return _hasError;
}

const String_t &
Error::getMessage() const
{
    return _message;
}

void Error::setError(String_t message)
{
    _message = message;
    _hasError = true;
}