#include "rule.hpp"
#include "production/empty_branch.hpp"
#include "production/uniform_branch.hpp"
#include "production/weighted_branch.hpp"

using namespace calyx;

Rule::Rule()
    : _production(new EmptyBranch()),
    _term(StringConverters::DEFAULT_STRING_CONVERTER.fromString(""))
{

}

Rule::Rule(String_t term, std::unique_ptr<ProductionBranch> production)
    : _production(std::move(production)),
    _term(term)
{
}

Rule::Rule(const Rule& other)
    : _production(other._production->cloneBranch()),
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
Rule::evaluate(Options& options, ErrorHolder& errors) const
{
    return _production->evaluate(options, errors);
}

std::optional<Expansion>
Rule::evaluateAt(int index, Options& options, ErrorHolder& errors) const
{
    return _production->evaluateAt(index, options, errors);
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
    _production = other._production->cloneBranch();

    return *this;
}