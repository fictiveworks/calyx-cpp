#pragma once
#include <map>
#include <vector>

#include "../filters.h"

namespace calyx
{
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

    class BaseFilters: public FiltersProvider
    {
    public:
        const std::map<String_t, Filter_t> getFilters(const Options& options) const override;

        static String_t uppercase(const String_t& input, const Options& options);

        BaseFilters();
        
        ~BaseFilters() override = default;
    };
    
}
