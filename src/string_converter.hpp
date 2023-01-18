#include <string>

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

struct StringConverters
{
    static const StringConverter<std::string> STD_STRING_CONVERTER;
};