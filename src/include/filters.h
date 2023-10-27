#pragma once
#include <map>

#include "string_converter.h"


namespace calyx
{
    class Options;

    /**
     * @brief Namespace for defining the Calyx Filters API.
     * <br><br>
     * Filters are functions that transform a production output. For example, they can take an evaluated string and make it all upper
     * case. Custom filters may be defined by users with the method Grammar::filters. 
     */
    namespace filters
    {
        /**
         * @brief A custom type for filters, provided for convenience. Filters transform the String_t input into a new string, for example
         * to return an uppercase copy. 
         */
        using Filter_t = std::function<String_t(const String_t&, const Options&)>;

        /**
         * @brief Creates a map of all the built in filters for Calyx. Used in the primary constructor of Grammar
         * for initializing the basic set of filters.
         * 
         * @param converter String converter for filter names 
         * @return Returns a new map by value of filter names to filter functions.
         */
        std::map<String_t, Filter_t> createBuiltinFilters(const StringConverter<String_t>& converter);

        /**
         * @brief Converts the input into upper case. 
         * @param input The input string
         * @param options Options for string conversion
         * @return Returns the input string as a new upper case string.
         */
        String_t uppercase(const String_t& input, const Options& options);
        
        /**
         * @brief Converts the input into lower case. 
         * @param input The input string
         * @param options Options for string conversion
         * @return Returns the input string as a new lower case string.
         */
        String_t lowercase(const String_t& input, const Options& options);
        
        /**
         * @brief Converts the input into a string that contains a base 10 number that is the length of the input.
         * @param input The input string
         * @param options Options for string conversion
         * @return Returns the length of the input as a new string containing a base 10 number.
         */
        String_t length(const String_t& input, const Options& options);
        
        /**
         * @brief Places emphasis markers on a string.
         *
         * For example if the input is "important thing", then the output will be "*important thing*".
         * @param input The input string
         * @param options Options for string conversion
         * @return Returns a new string that is the input string, but with * placed at the beginning and end of the string.
         */
        String_t emphasis(const String_t& input, const Options& options);
    }
}