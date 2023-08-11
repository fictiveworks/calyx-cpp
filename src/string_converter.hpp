#pragma once

#include <string>
#include <functional>

namespace calyx
{
    /**
     * The string type used by this version of Calyx. This should be preferred whenever interacting with strings in the Calyx API.
     *
     * If a std::string is ever needed in implementation, a String_t should first be converted to a std::string using the relevant
     * string converter, and then converted back to String_t when done with the std::string using the same.
     */
    typedef std::string String_t;

    /**
     * @brief The constructor for the default string converter for `String_t`s to be used by Calyx. If the String_t type is redefined
     * (e.g. to UE's FString), then the constructor this is referencing will also need to be changed.
     *
     * This is the constructor for the default string converter passed as an argument to any methods of Calyx that require
     * reading/modifying strings.
     */
    #define DEFAULT_STRING_CONVERTER() StdStringConverter()

    /**
     * @brief Converts a string-like type S into a std::string
     *
     * @tparam S A string-like that can be converted into a std::string
     */
    template <typename S>
    class StringConverter
    {
    public:
        StringConverter() = default;

        ~StringConverter() = default;

        /**
         * @brief Converts an instance of the string-like type S into a std::string.
         *
         * @param stringLike The string-like object to be converted
         * @return const std::string Returns a std::string derived from the stringLike input.
         * Note: this may be the same object as the input!
         */
        virtual const std::string toString(const S& stringLike) const = 0;

        /**
         * @brief Converts a std::string into an instance of the string-like type S
         *
         * @param stdString The std::string to be converted
         * @return const S Returns a string-like S derived from the std::string input.
         * Note: this may the same object as the input!
         */
        virtual const S fromString(const std::string& stdString) const = 0;
    };

    class StdStringConverter : public StringConverter<std::string>
    {
    public:
        StdStringConverter() = default;

        ~StdStringConverter() = default;

        /**
         * @brief Converts an instance of the string-like std::string into a std::string.
         *
         * @param stringLike The string-like object to be converted
         * @return const std::string Returns the stringLike input as it is already a std::string
         */
        virtual const std::string toString(const std::string& stringLike) const override;

        /**
         * @brief Converts a std::string into an instance of the string-like std::string
         *
         * @param stdString The std::string to be converted
         * @return const std::string Returns the stdString input as it is already a string-like std::string
         */
        virtual const std::string fromString(const std::string& stdString) const override;
    };

    /**
     * @brief A C-style string type (aka a char pointer)
     */
    typedef char* C_Str;

    /**
     * @brief A demonstration of how much I hate myself.
     *
     * This is only for demonstration purposes, of how one might define string converters for other string-like types.
     * It should probably never actually be used.
     */
    class CStrConverter : public StringConverter<C_Str>
    {
    public:
        CStrConverter() = default;

        ~CStrConverter() = default;

        /**
         * @brief Converts an instance of the string-like char* into a std::string.
         *
         * @param stringLike The string-like char* to be converted
         * @return const std::string Returns a std::string with the char* as its char buffer
         */
        virtual const std::string toString(const C_Str& stringLike) const override;

        /**
         * @brief Converts a std::string into an instance of a char*.
         *
         * Note that the returned pointer will be allocated on the heap, and the caller is responsible for deleting the pointer.
         *
         * @param stdString The std::string to be converted
         * @return const std::string Returns a copy of the string buffer as a char*
         */
        virtual const C_Str fromString(const std::string& stdString) const override;

    };
}
