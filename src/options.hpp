#pragma once

#include <functional>
#include <random>
#include <memory>
#include "include/string_converter.hpp"
#include "errors.hpp"


namespace calyx
{

    class ErrorHolder;

    class Options : public StringConverter<String_t>
    {

    public:
        static const bool DEFAULT_STRICT;


        /**
         * @brief Construct a new Options object with a default random number generator
         *
         * @param strict Determines if the parser should throw an error when encountering an undefined key
         * @param converter The string converter to use
         */
        Options(bool strict = DEFAULT_STRICT, std::unique_ptr<StringConverter<String_t>> converter = std::unique_ptr<StringConverter<String_t>>(new DEFAULT_STRING_CONVERTER()));

        /**
         * @brief Construct a new Options object with a specified random seed
         *
         * @param seed The random seed to use
         * @param strict Determines if the parser should throw an error when encountering an undefined key
         * @param converter The string converter to use
         */
        Options(unsigned int seed, bool strict = DEFAULT_STRICT, std::unique_ptr<StringConverter<String_t>> converter = std::unique_ptr<StringConverter<String_t>>(new DEFAULT_STRING_CONVERTER()));

        /**
         * @brief Construct a new Options object with a specific random number generator
         *
         * @param rng The random number generator to use
         * @param strict Determines if the parser should throw an error when encountering an undefined key
         * @param converter The string converter to use
         */
        Options(std::mt19937 rng, bool strict = DEFAULT_STRICT, std::unique_ptr<StringConverter<String_t>> converter = std::unique_ptr<StringConverter<String_t>>(new DEFAULT_STRING_CONVERTER()));

        Options(const Options& old) = delete;

        ~Options() override = default;

        /**
         * @brief Generates a random number
         *
         * @return int Returns a random int
         */
        int randInt();

        /**
         * @brief Generates a random double between 0 and 1
         *
         * @return double Returns a random double between 0 and 1
         */
        double randDouble();

        /**
         * @brief Generated a random number with a maximum bound and ignores errors.
         *
         * **Only use this when confident that errors will never be generated!**
         *
         * Max must be greater than 0!
         *
         * @param max The maximum bound of the random number (exclusive)
         * @return int Returns a random int between 0 (inclusive) and max (exclusive)
         */
        int randInt(int max);

        /**
         * @brief Generated a random number with a maximum bound
         *
         * Max must be greater than 0.
         *
         * @param max The maximum bound of the random number (exclusive)
         * @param errorHolder Reference to error holder
         * @return int Returns a random int between 0 (inclusive) and max (exclusive)
         */
        int randInt(int max, ErrorHolder& errorHolder);


        /**
         * @brief Generated a random number with minimum and maximum bounds, and ignores errors.
         *
         * **Only use this when confident that errors will never be generated!**
         *
         * Max must be greater than 0 and min!
         *
         * @param min The minimum bound of the random number (inclusive)
         * @param max The maximum bound of the random number (exclusive)
         * @return int Returns a random int between min (inclusive) and max (exclusive)
         */
        int randInt(int min, int max);

        /**
         * @brief Generated a random number with minimum and maximum bounds
         *
         * Max must be greater than 0 and min.
         *
         * @param min The minimum bound of the random number (inclusive)
         * @param max The maximum bound of the random number (exclusive)
         * @param errorHolder Reference to error holder
         * @return int Returns a random int between min (inclusive) and max (exclusive)
         */
        int randInt(int min, int max, ErrorHolder& errorHolder);

        /**
         * @brief Check whether to use strict-rule checking for expansion of unknown rules
         *
         * @return true
         * @return false
         */
        bool isStrict() const;

        /**
         * @brief Converts a string-like object into a concrete, std::string.
         *
         * Delegates this functionality to {@link #_converter}.
         *
         * @param stringLike The string-like object to be converted
         * @return const std::string The std::string equivalent of {@code stringLike}
         */
        std::string toString(const String_t& stringLike) const override;

        /**
         * @brief Converts a concrete, std::string into a string-like object.
         *
         * Delegates this functionality to {@link #_converter}.
         *
         * @param stdString The std::string to be converted
         * @return const String_t The String_t equivalent of the {@code stdString}
         */
        String_t fromString(const std::string& stdString) const override;

    private:
        const bool _strict;
        std::mt19937 _rng;
        const std::unique_ptr<StringConverter<String_t>> _converter;
    };
}