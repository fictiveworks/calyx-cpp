#pragma once

#include <string>
#include <vector>
#include <map>

#include "string_converter.hpp"
#include "options.hpp"

namespace calyx
{
    enum Exp
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
        Expansion(const Expansion &old);

        Expansion(Exp symbol, Expansion tail);

        Expansion(Exp symbol, String_t term);

        Expansion(Exp symbol, std::vector<Expansion> tail);

        Expansion &operator=(const Expansion &other);

        String_t flatten(Options &options) const;

        void collectAtoms(String_t &concat) const;

    private:
        std::vector<Expansion> _tail;
        String_t _term;
        Exp _symbol;
    };
}
