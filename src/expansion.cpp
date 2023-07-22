#include "expansion.hpp"
#include <vector>
#include <memory>


using namespace calyx;

Expansion::Expansion(const Expansion& old)
    : _symbol(old._symbol),
    _term(old._term)
{
    for (const auto& ptr : old._tail) {
        _tail.push_back(std::make_unique<Expansion>(*ptr));
    }
}

Expansion::Expansion(Exp symbol, String_t term)
    : _symbol(symbol),
    _term(term),
    _tail()
{
}

Expansion::Expansion(Exp symbol, std::unique_ptr<Expansion> tail)
    : _symbol(symbol),
    _term()
{
    _tail.push_back(std::move(tail));
}

Expansion::Expansion(Exp symbol, std::vector<std::unique_ptr<Expansion>> tail)
    : _symbol(symbol),
    _term(),
    _tail(std::move(tail))
{

}



Expansion&
Expansion::operator=(const Expansion& other)
{
    if (this != &other) {
        _symbol = other._symbol;
        _term = other._term;

        // Clear the existing _tail vector
        _tail.clear();

        // Copy the elements of other._tail into _tail
        for (const auto& ptr : other._tail) {
            _tail.push_back(std::make_unique<Expansion>(*ptr));
        }
    }

    return *this;
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
    if (_symbol == ATOM) {
        concat += _term;
    }
    else {
        for (const auto& expansions : _tail) {
            if (expansions != nullptr) {
                expansions->collectAtoms(concat);
            }
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