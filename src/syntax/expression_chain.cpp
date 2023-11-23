#include "expression_chain.h"

#include <numeric>

#include "expression_node.h"
#include "../registry.h"

using namespace calyx;

ExpressionChain::ExpressionChain(String_t ruleName, std::vector<std::string> components):
    _ruleName(std::move(ruleName)),
    _components(std::move(components))
{
}

std::shared_ptr<Production>
ExpressionChain::parse(const std::vector<std::string>& components, const Registry& registry, ErrorHolder& errors)
{
    String_t ruleName = components[0];

    if (ExpressionNode::isSigil(ruleName[0]))
    {
        const Options& options = registry.getOptions();
        std::string stdRuleName = options.toString(ruleName);
        stdRuleName = stdRuleName.substr(1, std::string::npos);
        ruleName = options.fromString(stdRuleName);
    }
    
    std::vector<String_t> filterNames;
    for (std::size_t i = 1; i < components.size(); i++)
    {
        filterNames.push_back(components[i]);
    }

    return std::make_shared<ExpressionChain>(std::move(ruleName), std::move(filterNames));
}

std::optional<Expansion>
ExpressionChain::evaluate(Registry& registry, Options& options, ErrorHolder& errors) const
{
    const std::shared_ptr<Rule> rule = registry.expand(_ruleName, errors);

    if (rule == nullptr || errors.hasError())
    {
        return {};
    }

    const std::optional<Expansion> eval = rule->evaluate(registry, options, errors);
    if (!eval || errors.hasError())
    {
        return {};
    }

    const String_t initial = Expansion(Exp::EXPRESSION, eval->getTail()).flatten(options);

    const std::string modified = std::accumulate(
        _components.begin(), _components.end(), initial,
        [&](const std::string& accumulator, const std::string& filterName) -> String_t {
            const std::optional<const filters::Filter_t> filter = registry.getFilter(filterName);

            if (filter)
            {
                return (*filter)(accumulator, options);
            }

            errors.setError(Errors::undefinedFilter(filterName, options));
            return accumulator;
        }
    );

    return Expansion(Exp::EXPRESSION, Expansion(Exp::ATOM, options.fromString(modified)));
}