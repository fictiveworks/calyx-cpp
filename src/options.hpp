#pragma once

#include <functional>
#include <random>
#include "string_converter.hpp"

namespace calyx
{
    struct Options
    {

    public:
        static const bool DEFAULT_STRICT;

        const bool _strict;
        const std::function<int()> _rng;
        const StringConverter<String_t> &_converter;

        /**
         * @brief Construct a new Options object with a default random number generator
         *
         * @param strict Determines if the parser should throw an error when encountering an undefined key
         * @param converter The string converter to use
         */
        Options(bool strict = DEFAULT_STRICT, const StringConverter<String_t> &converter = StringConverters::CALYX_STRING_CONVERTER);

        /**
         * @brief Construct a new Options object with a specified random seed
         *
         * @param seed The random seed to use
         * @param strict Determines if the parser should throw an error when encountering an undefined key
         * @param converter The string converter to use
         */
        Options(int seed, bool strict = DEFAULT_STRICT, const StringConverter<String_t> &converter = StringConverters::CALYX_STRING_CONVERTER);

        /**
         * @brief Construct a new Options object with a specific random number generator
         *
         * @param rng The random number generator to use
         * @param strict Determines if the parser should throw an error when encountering an undefined key
         * @param converter The string converter to use
         */
        Options(std::function<int()> rng, bool strict = DEFAULT_STRICT, const StringConverter<String_t> &converter = StringConverters::CALYX_STRING_CONVERTER);

        ~Options();
    };
}