#include "filters.h"

using namespace calyx;

BaseFilters::BaseFilters()
{
}

const std::map<String_t, Filter_t>
BaseFilters::getFilters(const Options& options) const
{
    return std::map<String_t, Filter_t> {
        { options.fromString("uppercase"), &uppercase },
    };
}


String_t
BaseFilters::uppercase(const String_t& input, const Options& options)
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