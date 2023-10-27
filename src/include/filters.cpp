#include "filters.h"

#include <sstream>

#include "../production/uniform_branch.h"

using namespace calyx;


const std::map<String_t, Filter_t>&
FiltersProvider::getFilters() const
{
    return _filters;
}


BuiltinFilters::BuiltinFilters():
    BuiltinFilters(DEFAULT_STRING_CONVERTER())
{
}

BuiltinFilters::BuiltinFilters(const StringConverter<String_t>& converter)
{
    auto builtin = std::map<String_t, Filter_t> {
        { converter.fromString("uppercase"), &uppercase },
        { converter.fromString("lowercase"), &lowercase },
        {
            converter.fromString("emphasis"),
            [](auto s, auto o) {
                std::ostringstream oss;

                oss << o.fromString("*")
                    << o.toString(s)
                    << o.fromString("*");

                return o.fromString(oss.str());
            }
        },
        {
            converter.fromString("length"),
            [](auto s, auto o) {
                std::ostringstream oss;
                oss << o.toString(s).length();

                return o.fromString(oss.str());
            }
        }
    };

    _filters.insert(builtin.begin(), builtin.end());
}


String_t
BuiltinFilters::uppercase(const String_t& input, const Options& options)
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
BuiltinFilters::lowercase(const String_t& input, const Options& options)
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