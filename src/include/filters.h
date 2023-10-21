#pragma once
#include <map>

#include "string_converter.h"


namespace calyx
{
    class Options;

    using Filter_t = String_t(*)(const String_t&, const Options&);
    
    class FiltersProvider
    {
    public:
        virtual const std::map<String_t, Filter_t> getFilters(
            const Options& options
        ) const = 0;

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
    };

    class DefaultFilters: public FiltersProvider
    {
    public:
        const std::map<String_t, Filter_t> getFilters(const Options& options) const override;

        static String_t uppercase(const String_t& input, const Options& options);
        
        static String_t lowercase(const String_t& input, const Options& options);
        
        static String_t length(const String_t& input, const Options& options);

        static String_t emphasis(const String_t& input, const Options& options);

        DefaultFilters();
        
        ~DefaultFilters() override = default;
    };
    
}
