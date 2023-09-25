#pragma once

#include "calyx.h"

namespace calyx
{
    class Grammar
    {
    public:
        Grammar() noexcept;

        explicit Grammar(Options opts) noexcept;

        explicit Grammar(bool strict = Options::DEFAULT_STRICT) noexcept;

        explicit Grammar(std::mt19937 rng, bool strict = Options::DEFAULT_STRICT) noexcept;

        explicit Grammar(int seed, bool strict = Options::DEFAULT_STRICT) noexcept;
        
        Grammar(Grammar&& old) noexcept;
        
        Grammar(const Grammar& other) = delete;

        ~Grammar() = default;

        Grammar& operator=(Grammar&& old) noexcept;
        
        Grammar& operator=(const Grammar& old) = delete;

        void start(String_t production) noexcept;

        void start(std::vector<String_t> production) noexcept;

        void rule(String_t term, String_t production) noexcept;

        void rule(std::vector<String_t> term, std::vector<String_t> production) noexcept;

        Result generate() const noexcept;

    private:
        Registry _registry;
    };
}