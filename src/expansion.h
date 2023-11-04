#pragma once

#include <string>
#include <vector>
#include <vector>

#include "calyx/string_converter.h"
#include "calyx/options.h"

namespace calyx
{
    enum class Exp
    {
        RESULT,
        UNIFORM_BRANCH,
        WEIGHTED_BRANCH,
        EMPTY_BRANCH,
        AFFIX_TABLE,
        TEMPLATE,
        EXPRESSION,
        ATOM,
        MEMO,
        UNIQ
    };

    class Expansion
    {
    public:
        Expansion(const Expansion& old) = default;

        Expansion(Exp symbol, String_t term);

        Expansion(Exp symbol, Expansion tail);

        Expansion(Exp symbol, std::vector<Expansion> tail);

        Expansion& operator=(const Expansion& other);
        
        void collectAtoms(String_t& concat) const;
        
        [[nodiscard]] String_t flatten(const Options& options) const;

        [[nodiscard]] const String_t& getTerm() const;

        [[nodiscard]] const Exp& getSymbol() const;

        [[nodiscard]] const std::vector<Expansion>& getTail() const;

    private:
        std::vector<Expansion> _tail;
        String_t _term;
        Exp _symbol;
    };
}
