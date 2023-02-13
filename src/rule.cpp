#include "rule.hpp"
#include "production/empty_branch.hpp"

using namespace calyx;

Rule::Rule(String_t term, ProductionBranch& production)
    : _production(production),
      _term(term)
{
}

Expansion Rule::evaluate(Options &options) const
{
    return _production.evaluate(options);
}

Expansion Rule::evaluateAt(int index, Options &options) const
{
    return _production.evaluateAt(index, options);
}

Rule &Rule::operator=(const Rule &other)
{
    _term = other._term;
    _production = other._production;

    return *this;
}