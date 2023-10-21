#include "filters.h"

#include <sstream>

#include "../production/uniform_branch.h"

using namespace calyx;

DefaultFilters::DefaultFilters()
{
}

const std::map<String_t, Filter_t>
DefaultFilters::getFilters(const Options& options) const
{
    return std::map<String_t, Filter_t> {
        { options.fromString("uppercase"), &uppercase },
        { options.fromString("lowercase"), &lowercase },
        { options.fromString("emphasis"), &emphasis },
        { options.fromString("length"), &length }
    };
}


String_t
DefaultFilters::uppercase(const String_t& input, const Options& options)
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
DefaultFilters::lowercase(const String_t& input, const Options& options)
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
DefaultFilters::length(const String_t& input, const Options& options)
{
    std::ostringstream oss;
    oss << options.toString(input).length();

    return options.fromString(oss.str());
}

String_t
DefaultFilters::emphasis(const String_t& input, const Options& options)
{
    std::ostringstream oss;

    oss << options.fromString("*")
        << options.toString(input)
        << options.fromString("*");

    return options.fromString(oss.str());
}