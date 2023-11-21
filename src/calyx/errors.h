#pragma once

#include "string_converter.h"

namespace calyx
{

    /**
     * @brief Stores commonly used error message factories. Note that these are not proper error holders!
     *
     */
    struct Errors
    {
        static String_t undefinedRule(const String_t& symbol, const StringConverter<String_t>& converter);

        static String_t undefinedFilter(const String_t& symbol, const StringConverter<String_t>& converter);
    };

    /**
     * @brief Stores error messages to keep compatibility with UE and its lack of exceptions.
     *
     * Errors are generated by setting an error message with {@link ErrorHolder#setError}, and then early returning with an arbitrary default
     * value, or by returning an empty {@link std::optional} to force the caller to deal with it
     *
     * Error Holders should always be passed by reference.
     */
    class ErrorHolder
    {
    public:
        /**
         * @brief Construct a new Error object with no errors
         *
         */
        ErrorHolder() = default;

        ~ErrorHolder() = default;

        ErrorHolder(ErrorHolder&& old) = default;
        
        /**
         * @brief Errors should be passed by reference. The copy constructor is deleted to enforce this.
         *
         * @param old
         */
        ErrorHolder(const ErrorHolder& old) = delete;

        /**
         * @brief Errors should be passed by reference. The assignment operator is deleted to enforce this.
         *
         * @param other
         * @return Error&
         */
        ErrorHolder& operator=(const ErrorHolder& other) = delete;

        ErrorHolder& operator=(ErrorHolder&& old) noexcept;

        /**
         * @brief Returns true if the error holder has an error. Identical to checking hasError().  
         */
        explicit operator bool() const;

        /**
         * @brief Determine if errors are present. Errors are present if and only if {@link ErrorHolder#setError} has been called on this instance.
         *
         * @return true Returns true if an error is present
         * @return false Returns false if an error is not present
         */
        bool hasError() const;

        /**
         * @brief Get the error message.
         *
         * @return const String_t& Returns the message if an error is present, otherwise returns an empty string.
         */
        const String_t& getMessage() const;

        /**
         * @brief Sets an error message to the given message, and also sets hasError to true.
         *
         * @param message The error message
         */
        void setError(String_t message);

    private:
        String_t _message = DEFAULT_STRING_CONVERTER().fromString("");
        bool _hasError = false;
    };
}
