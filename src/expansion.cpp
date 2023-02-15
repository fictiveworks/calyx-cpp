#include "expansion.hpp"

using namespace calyx;

Expansion::Expansion(const Expansion& old)
    : _symbol(old._symbol),
    _term(old._term),
    _tail(old._tail)
{
}

Expansion::Expansion(Exp symbol, String_t term)
    : _symbol(symbol),
    _term(term),
    _tail()
{
}

Expansion::Expansion(Exp symbol, std::shared_ptr<Expansion> tail)
    : _symbol(symbol),
    _term(),
    _tail(1)
{
    _tail.push_back(tail);
}

Expansion::Expansion(Exp symbol, std::vector<std::shared_ptr<Expansion>> tail)
    : _symbol(symbol),
    _term(),
    _tail(tail)
{

}



Expansion&
Expansion::operator=(const Expansion& other)
{
    _symbol = other._symbol;
    _term = other._term;
    _tail = other._tail;
}

String_t
Expansion::flatten(Options& options) const
{
    String_t concat = options._converter.fromString("");
    collectAtoms(concat);
    return concat;
}

void Expansion::collectAtoms(String_t& concat) const
{
    if (_symbol == ATOM)
    {
        concat += _term;
    }
    else
    {
        for (auto expansions : _tail)
        {
            expansions->collectAtoms(concat);
        }
    }
}