#include "errors.hpp"
#include <string>
#include <sstream>

using namespace calyx;

ErrorHolder::ErrorHolder()
    : _message(StringConverters::DEFAULT_STRING_CONVERTER.fromString("")),
    _hasError(false)
{
}

String_t
Errors::undefinedRule(const String_t& symbol, const StringConverter<String_t>& converter)
{
    std::ostringstream stream;
    stream << "Undefined rule: '" << converter.toString(symbol) << "'";
    return converter.fromString(stream.str());
}

String_t
Errors::undefinedFilter(const String_t& symbol, const StringConverter<String_t>& converter)
{
    std::ostringstream stream;
    stream << "Undefined filter: '" << converter.toString(symbol) << "'";
    return converter.fromString(stream.str());
}

String_t
Errors::incorrectFilterSignature(const String_t& symbol, const StringConverter<String_t>& converter)
{
    std::ostringstream stream;
    stream << "Incorrect method signature for filter: '" << converter.toString(symbol) << "'";
    return converter.fromString(stream.str());
}

bool
ErrorHolder::hasError() const
{
    return _hasError;
}

const String_t&
ErrorHolder::getMessage() const
{
    return _message;
}

void
ErrorHolder::setError(String_t message)
{
    _message = message;
    _hasError = true;
}