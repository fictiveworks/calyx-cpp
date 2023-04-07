#include "rule.hpp"
#include "production/empty_branch.hpp"
#include "production/uniform_branch.hpp"

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

Rule 
Rule::build(String_t term, std::vector<String_t> productions, Registry& registry)
{
    UniformBranch production = UniformBranch::parse(productions, registry);
    return Rule(term, std::make_unique<UniformBranch>(std::move(production)));
}

// Rule 
// Rule::build(String_t term, std::map<String_t, int> productions, Registry& registry)
// {
    
// }

Expansion
Rule::evaluate(Options& options) const
{
    return _production->evaluate(options);
}

Expansion
Rule::evaluateAt(int index, Options& options) const
{
    return _production->evaluateAt(index, options);
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