#include "expansion.hpp"
#include <vector>
#include <memory>


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

Expansion::Expansion(Exp symbol, Expansion tail)
    : _symbol(symbol),
    _term()
{
    _tail.push_back(tail);
}

Expansion::Expansion(Exp symbol, std::vector<Expansion> tail)
    : _symbol(symbol),
    _term(),
    _tail(tail)
{

}

Expansion&
Expansion::operator=(const Expansion& other)
{
    if (this != &other) {
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
    if (_symbol == Exp::ATOM) {
        concat += _term;
    }
    else {
        for (const Expansion& expansions : _tail) {
            expansions.collectAtoms(concat);
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