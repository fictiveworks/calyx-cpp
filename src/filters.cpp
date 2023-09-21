#include "filters.h"
#include <cctype>

using namespace calyx;


String_t
filters::uppercase(String_t input, const Options& ops)
{
    std::string raw = ops.toString(input);

    std::ranges::transform(raw, raw.begin(), std::toupper);

    return ops.fromString(raw);
}
