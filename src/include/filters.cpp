#include "filters.h"

#include <sstream>
#include <algorithm>
#include "../production/uniform_branch.h"

using namespace calyx;

std::map<String_t, filters::Filter_t>
filters::createBuiltinFilters(const StringConverter<String_t>& converter)
{
    return std::map<String_t, Filter_t> {
        { converter.fromString("uppercase"), &uppercase },
        { converter.fromString("lowercase"), &lowercase },
        { converter.fromString("emphasis"), &emphasis },
        { converter.fromString("length"), &length }
    };
}

String_t
filters::uppercase(const String_t& input, const Options& options)
{
    std::string str = options.toString(input);

    std::ranges::transform(
        str.begin(), str.end(),
        str.begin(),
        [](const unsigned char c) {
            return std::toupper(c);
        }
    );

    return options.fromString(str);
}

String_t
filters::lowercase(const String_t& input, const Options& options)
{
    std::string str = options.toString(input);

    std::ranges::transform(
        str.begin(), str.end(),
        str.begin(),
        [](const unsigned char c) {
            return std::tolower(c);
        }
    );

    return options.fromString(str);
}

String_t
filters::length(const String_t& input, const Options& options)
{
    std::ostringstream oss;
    oss << options.toString(input).length();

    return options.fromString(oss.str());
}

String_t
filters::emphasis(const String_t& input, const Options& options)
{
    std::ostringstream oss;

    oss << options.fromString("*")
        << options.toString(input)
        << options.fromString("*");

    return options.fromString(oss.str());
}