#pragma once

#include <optional>

#include "calyx/options.h"

namespace calyx
{
    class Cycle
    {
    public:

        Cycle() noexcept;
        
        static std::optional<Cycle> create(std::size_t count, StringConverter<String_t>& converter, ErrorHolder& errs) noexcept;

        void shuffle(Options& options) noexcept;

        std::size_t poll(Options& options) noexcept;

        const std::vector<std::size_t>& getSequence() const noexcept;

        // rule of 5 stuff :D 
        
        Cycle(const Cycle& other) noexcept = default;

        Cycle(Cycle&& other) noexcept;

        Cycle& operator=(const Cycle& other) noexcept;

        Cycle& operator=(Cycle&& other) noexcept;

        ~Cycle() = default;

    private:
        std::size_t _count;
        std::size_t _index;
        std::vector<std::size_t> _sequence;

        Cycle(size_t count) noexcept;
        void populateSequence() noexcept;
    };
}