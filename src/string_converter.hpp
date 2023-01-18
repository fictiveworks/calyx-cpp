#include <string>

/**
 * The string type used by this version of Calyx. This should be preferred whenever interacting with strings in the Calyx API.
 *
 * If a std::string is ever needed in implementation, CalyStrings should first be converted to a std::string using the relevant 
 * string converter, and then converted back to CalyxString when done with the std::string using the same.
 */
typedef std::string CalyxString;

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
     * @return const std::string& Returns a reference to a std::string derived from the stringLike input. 
     * Note: this may be the same object as the input! 
     */
    virtual const std::string& toString(const S& stringLike) const = 0;

    /**
     * @brief Converts a std::string into an instance of the string-like type S
     * 
     * @param stdString The std::string to be converted
     * @return const S& Returns a reference to the string-like S derived from the std::string input.
     * Note: this may the same object as the input!
     */
    virtual const S& fromString(const std::string& stdString) const = 0;

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
     * @return const std::string& Returns the stringLike input as it is already a std::string
     */
    virtual const std::string& toString(const std::string& stringLike) const override;

    /**
     * @brief Converts a std::string into an instance of the string-like std::string
     * 
     * @param stdString The std::string to be converted
     * @return const std::string& Returns the stdString input as it is already a string-like std::string
     */
    virtual const std::string& fromString(const std::string& stdString) const override;

};

struct StringConverters
{
    static const StdStringConverter STD_STRING_CONVERTER;

    static const StringConverter<CalyxString>& DEFAULT_STRING_CONVERTER;
};