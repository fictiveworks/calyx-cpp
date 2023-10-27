#pragma once
#include <map>

#include "string_converter.h"


namespace calyx
{
    class Options;

    using Filter_t = std::function<String_t(const String_t&, const Options&)>;
    
    class FiltersProvider
    {
    public:
        const std::map<String_t, Filter_t>& getFilters() const;

        FiltersProvider() = default;
        
        FiltersProvider(const FiltersProvider& other) = delete;

        FiltersProvider(FiltersProvider&& other) noexcept
        {
        }

        FiltersProvider& operator=(const FiltersProvider& other) = delete;

        FiltersProvider& operator=(FiltersProvider&& other) noexcept
        {
            if (this == &other)
                return *this;
            return *this;
        }

        virtual ~FiltersProvider() = default;
    protected:
        std::map<String_t, Filter_t> _filters;
    };

    class BuiltinFilters: public FiltersProvider
    {
    public:
        
        static String_t uppercase(const String_t& input, const Options& options);
        
        static String_t lowercase(const String_t& input, const Options& options);
        
        static String_t length(const String_t& input, const Options& options);

        static String_t emphasis(const String_t& input, const Options& options);
        
        BuiltinFilters();

        BuiltinFilters(const StringConverter<String_t>& converter);
        
        ~BuiltinFilters() override = default;
    };
    
}
