#pragma once
#include <functional>

#include "include/string_converter.h"
#include "options.h"

namespace calyx
{
    using Filter_t = String_t(*)(String_t, const Options&);

    namespace filters
    {
        String_t uppercase(String_t input, const Options& ops);
    }

}