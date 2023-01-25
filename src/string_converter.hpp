#include <string>

namespace calyx
{
    /**
     * The string type used by this version of Calyx. This should be preferred whenever interacting with strings in the Calyx API.
     *
     * If a std::string is ever needed in implementation, CalyStrings should first be converted to a std::string using the relevant
     * string converter, and then converted back to String_t when done with the std::string using the same.
     */
    typedef std::string String_t;

    /**
     * @brief Converts a string-like type S into a std::string
     *
     * @tparam S A string-like that can be converted into a std::string
     */
    template <typename S>
    class StringConverter
    {
    public:
        StringConverter();

        ~StringConverter();

        /**
         * @brief Converts an instance of the string-like type S into a std::string.
         *
         * @param stringLike The string-like object to be converted
         * @return const std::string Returns a std::string derived from the stringLike input.
         * Note: this may be the same object as the input!
         */
        virtual const std::string toString(const S &stringLike) const = 0;

        /**
         * @brief Converts a std::string into an instance of the string-like type S
         *
         * @param stdString The std::string to be converted
         * @return const S Returns a string-like S derived from the std::string input.
         * Note: this may the same object as the input!
         */
        virtual const S fromString(const std::string &stdString) const = 0;
    };

    class StdStringConverter : public StringConverter<std::string>
    {
    public:
        StdStringConverter();

        ~StdStringConverter();

        /**
         * @brief Converts an instance of the string-like std::string into a std::string.
         *
         * @param stringLike The string-like object to be converted
         * @return const std::string Returns the stringLike input as it is already a std::string
         */
        virtual const std::string toString(const std::string &stringLike) const override;

        /**
         * @brief Converts a std::string into an instance of the string-like std::string
         *
         * @param stdString The std::string to be converted
         * @return const std::string Returns the stdString input as it is already a string-like std::string
         */
        virtual const std::string fromString(const std::string &stdString) const override;
    };

    struct StringConverters
    {
        /**
         * @brief The converter for standard strings.
         *
         */
        static const StdStringConverter STD_STRING_CONVERTER;

        /**
         * @brief A reference to a string converter for `String_t`s. If the String_t type is redefined
         * (e.g. to UE's FString), then the object this is referencing will also need to be changed.
         *
         * This is the default string converter passed as an argument to any methods of Calyx that require
         * reading/modifying strings. It should do this in the `Options` argument for API methods.
         */
        static const StringConverter<String_t> &CALYX_STRING_CONVERTER;
    };
}
