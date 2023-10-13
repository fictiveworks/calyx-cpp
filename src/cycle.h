#pragma once

#include <optional>

#include "options.h"

namespace calyx
{
    class Cycle
    {
    public:

        Cycle() noexcept;
        
        static std::optional<Cycle> create(size_t count, StringConverter<String_t>& converter, ErrorHolder& errs) noexcept;

        void shuffle(Options& options) noexcept;

        int poll(Options& options) noexcept;

        // rule of 5 stuff :D 
        
        Cycle(const Cycle& other) noexcept = default;

        Cycle(Cycle&& other) noexcept;

        Cycle& operator=(const Cycle& other) noexcept;

        Cycle& operator=(Cycle&& other) noexcept;

        ~Cycle() = default;

    private:
        size_t _count;
        size_t _index;
        std::vector<int> _sequence;

        Cycle(size_t count) noexcept;
        void populateSequence() noexcept;
    };
}