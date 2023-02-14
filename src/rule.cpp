#include "rule.hpp"
#include "production/empty_branch.hpp"

using namespace calyx;

Rule::Rule()
    : _production(std::make_unique<ProductionBranch>(EmptyBranch())),
    _term(StringConverters::DEFAULT_STRING_CONVERTER.fromString(""))
{

}

Rule::Rule(String_t term, ProductionBranch& production)
    : _production(std::make_unique<ProductionBranch>(production)),
    _term(term)
{
}

Rule::Rule(const Rule& other)
    : _production(other._production->cloneBranch()),
    _term(other._term)
{
}

Rule::~Rule()
{

}

Expansion Rule::evaluate(Options& options) const
{
    return _production->evaluate(options);
}

Expansion Rule::evaluateAt(int index, Options& options) const
{

    return _production->evaluateAt(index, options);
}

Rule& Rule::operator=(const Rule& other)
{
    _term = other._term;
    _production = other._production->cloneBranch();

    return *this;
}