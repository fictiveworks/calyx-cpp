#include "rule.h"

#include "production/empty_branch.h"
#include "production/uniform_branch.h"
#include "production/weighted_branch.h"

using namespace calyx;

Rule::Rule(String_t term, std::shared_ptr<ProductionBranch> production)
    : _production(production),
    _term(term)
{
}

Rule::Rule(const Rule& other)
    : _production(other._production),
    _term(other._term)
{
}

Rule
Rule::empty(String_t term)
{
    return Rule(term, std::make_unique<EmptyBranch>());
}

std::optional<Rule>
Rule::build(String_t term, std::vector<String_t> productions, const Registry& registry, ErrorHolder& errors)
{
    std::optional<UniformBranch> branch = UniformBranch::parse(productions, registry, errors);

    if (!branch)
    {
        return {};
    }

    return Rule(term, std::make_unique<UniformBranch>(*branch));
}

std::optional<Expansion>
Rule::evaluate(
    Registry& registry,
    Options& options,
    ErrorHolder& errors
) const
{
    return _production->evaluate(registry, options, errors);
}

std::optional<Expansion>
Rule::evaluateAt(
    int index,
    Registry& registry,
    Options& options,
    ErrorHolder& errors
) const
{
    return _production->evaluateAt(index, registry, options, errors);
}

size_t
Rule::length() const
{
    return _production->length();
}

Rule&
Rule::operator=(const Rule& other)
{
    _term = other._term;
    _production = other._production;

    return *this;
}