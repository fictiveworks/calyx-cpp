#include "expansion.h"
#include <vector>
#include <memory>


using namespace calyx;

Expansion::Expansion(Exp symbol, String_t term):
    _term(std::move(term)),
    _symbol(symbol)
{
}

Expansion::Expansion(Exp symbol, Expansion tail):
    _symbol(symbol)
{
    _tail.push_back(std::move(tail));
}

Expansion::Expansion(Exp symbol, std::vector<Expansion> tail):
    _tail(std::move(tail)),
    _symbol(symbol)
{
}

Expansion&
Expansion::operator=(const Expansion& other)
{
    if (this != &other)
    {
        _symbol = other._symbol;
        _term = other._term;
        _tail = other._tail;
    }

    return *this;
}

String_t
Expansion::flatten(const Options& options) const
{
    String_t concat = options.fromString("");
    collectAtoms(concat);
    return concat;
}

void Expansion::collectAtoms(String_t& concat) const
{
    if (_symbol == Exp::ATOM)
    {
        concat += _term;
    }
    else
    {
        for (const Expansion& child : _tail)
        {
            child.collectAtoms(concat);
        }
    }
}

const String_t&
Expansion::getTerm() const
{
    return _term;
}

const Exp&
Expansion::getSymbol() const
{
    return _symbol;
}

const std::vector<Expansion>&
Expansion::getTail() const
{
    return _tail;
}