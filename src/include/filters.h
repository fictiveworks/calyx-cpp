#pragma once
#include <map>

#include "string_converter.h"


namespace calyx
{
    class Options;

    namespace filters
    {
        /**
         * @brief A custom type for filters. A filter is a function that takes two constant references to a String_t and Options,
         * and returns a String_t value. The purpose of a filter is to transform the String_t input into a new string, for example
         * to return an uppercase copy.
         *
         * See FiltersProvider for more details.
         */
        using Filter_t = std::function<String_t(const String_t&, const Options&)>;

        std::map<String_t, Filter_t> createBuiltinFilters(const StringConverter<String_t>& converter);

        String_t uppercase(const String_t& input, const Options& options);

        String_t lowercase(const String_t& input, const Options& options);

        String_t length(const String_t& input, const Options& options);

        String_t emphasis(const String_t& input, const Options& options);
    }
}