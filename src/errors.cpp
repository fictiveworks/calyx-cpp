#include "errors.hpp"
#include <string>
#include <sstream>

using namespace calyx;

ErrorHolder::ErrorHolder(const Options& options)
    : _message(options.fromString("")),
    _hasError(false)
{
}

String_t
Errors::undefinedRule(const String_t& symbol, const Options& options)
{
    std::ostringstream stream;
    stream << "Undefined rule: '" << options.toString(symbol) << "'";
    return options.fromString(stream.str());
}

String_t
Errors::undefinedFilter(const String_t& symbol, const Options& options)
{
    std::ostringstream stream;
    stream << "Undefined filter: '" << options.toString(symbol) << "'";
    return options.fromString(stream.str());
}

String_t
Errors::incorrectFilterSignature(const String_t& symbol, const Options& options)
{
    std::ostringstream stream;
    stream << "Incorrect method signature for filter: '" << options.toString(symbol) << "'";
    return options.fromString(stream.str());
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