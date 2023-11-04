#pragma once

#include <random>
#include <memory>
#include <stdint.h>
#include "string_converter.h"
#include "errors.h"


namespace calyx
{
    /**
     * @brief Class for dealing with various options relating to grammars. For example, whether to run grammars in 'loose' or 'strict'
     * mode, and what the primary string converter is. This class provides its own random number generation API, and also
     * can convert `std::string`s to `String_t`s. 
     */
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
        Options(
            bool strict = DEFAULT_STRICT,
            std::unique_ptr<StringConverter> converter = std::make_unique<DEFAULT_STRING_CONVERTER>()
        );

        /**
         * @brief Construct a new Options object with a specified random seed
         *
         * @param seed The random seed to use
         * @param strict Determines if the parser should throw an error when encountering an undefined key
         * @param converter The string converter to use
         */
        Options(
            std::uint32_t seed,
            bool strict = DEFAULT_STRICT,
            std::unique_ptr<StringConverter> converter = std::make_unique<DEFAULT_STRING_CONVERTER>()
        );

        /**
         * @brief Construct a new Options object with a specific random number generator
         *
         * @param rng The random number generator to use
         * @param strict Determines if the parser should throw an error when encountering an undefined key
         * @param converter The string converter to use
         */
        Options(
            std::mt19937 rng,
            bool strict = DEFAULT_STRICT,
            std::unique_ptr<StringConverter> converter = std::make_unique<DEFAULT_STRING_CONVERTER>()
        );

        Options(Options&& other) noexcept;

        ~Options() override = default;

        Options(const Options& old) = delete;

        Options& operator=(const Options& other) noexcept = delete;

        Options& operator=(Options&& old) noexcept;

        /**
         * @brief Generated a random number with minimum and maximum bounds, and ignores errors.
         *
         * **Only use this when confident that errors will never be generated!**
         *
         * Max must be greater than 0 and min!
         *
         * @tparam T The integer type of the number to be generated.
         * @param min The minimum bound of the random number (inclusive)
         * @param max The maximum bound of the random number (exclusive)
         * @return int Returns a random int between min (inclusive) and max (exclusive)
         */
        template <typename T>
        T randomInteger(T min, T max)
        {
            ErrorHolder errs;
            return randomInteger(min, max, errs);
        }

        /**
         * @brief Shuffles a vector of items, in place.
         * 
         * @tparam T The type of the items in the vector 
         * @param items The vector to shuffle
         */
        template <typename T>
        void shuffle(std::vector<T>& items)
        {
            std::shuffle(items.begin(), items.end(), _rng);
        }

        /**
         * @brief Generated a random number with a maximum bound and ignores errors.
         *
         * **Only use this when confident that errors will never be generated!**
         *
         * Max must be greater than 0!
         *
         * @tparam T The integer type of the number to be generated.
         * @param max The maximum bound of the random number (exclusive)
         * @return int Returns a random int between 0 (inclusive) and max (exclusive)
         */
        template <typename T>
        T randomInteger(T max)
        {
            ErrorHolder errs;
            return randomInteger(max, errs);
        }

        /**
         * @brief Generated a random number with minimum and maximum bounds
         *
         * Max must be greater than 0 and min.
         *
         * @tparam T The integer type of the number to be generated.
         * @param min The minimum bound of the random number (inclusive)
         * @param max The maximum bound of the random number (exclusive)
         * @param errorHolder Reference to error holder
         * @return int Returns a random int between min (inclusive) and max (exclusive)
         */
        template <typename T>
        T randomInteger(T min, T max, ErrorHolder& errorHolder)
        {
            static_assert(std::is_integral_v<T>, "T must be an integral (integer) type");
            if (max <= 0)
            {
                String_t msg = _converter->fromString("Max bound must be positive");
                errorHolder.setError(msg);
                return 0;
            }

            if (max <= min)
            {
                String_t msg = _converter->fromString("Max bound must be greater than min!");
                errorHolder.setError(msg);
                return 0;
            }
            std::uniform_int_distribution distribution(min, max - 1);

            return distribution(_rng);
        }

        /**
         * @brief Generated a random number with a maximum bound
         *
         * Max must be greater than 0.
         *
         * @tparam T The integer type of the number to be generated.
         * @param max The maximum bound of the random number (exclusive)
         * @param errorHolder Reference to error holder
         * @return int Returns a random int between 0 (inclusive) and max (exclusive)
         */
        template <typename T>
        T randomInteger(T max, ErrorHolder& errorHolder)
        {
            static_assert(std::is_integral_v<T>, "T must be an integral (integer) type");
            if (max <= 0)
            {
                const String_t msg = _converter->fromString("Max bound must be positive");
                errorHolder.setError(msg);
                return 0;
            }

            std::uniform_int_distribution<T> distribution(0, max - 1);

            return distribution(_rng);
        }

        /**
         * @brief Generates a random number
         *
         * @tparam T The integer type of the number to be generated.
         * @return int Returns a random int
         */
        template <typename T>
        T randomInteger()
        {
            static_assert(std::is_integral_v<T>, "T must be an integral (integer) type");
            return _rng();
        }

        /**
         * @brief Generates a random real number between 0 and 1.
         *
         * @tparam T The real type of the number to be generated. Must be float or double.
         * @return double Returns a random real number between 0 and 1
         */
        template <typename T>
        T randomReal()
        {
            static_assert(std::is_floating_point_v<T>, "T must be a real number type (eg double or float)");
            std::uniform_real_distribution distribution(
                0.0, 1.0
            );

            return distribution(_rng);
        }

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
        bool _strict;
        std::mt19937 _rng;
        std::unique_ptr<StringConverter> _converter;
    };
}