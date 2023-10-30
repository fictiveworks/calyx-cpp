#include "expression_chain.h"

#include <numeric>

#include "../registry.h"

using namespace calyx;

ExpressionChain::ExpressionChain(std::vector<std::string> components):
    _components(std::move(components))
{
}

std::optional<Expansion>
ExpressionChain::evaluate(Registry& registry, Options& options, ErrorHolder& errors) const
{
    const std::shared_ptr<Rule> rule = registry.expand(_components[0], errors);

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

    const auto begin = std::next(_components.begin());

    const std::string modified = std::accumulate(
        begin, _components.end(), initial,
        [&](const std::string& accumulator, const std::string& filterName) -> String_t {
            const std::optional<const filters::Filter_t> filter = registry.getFilter(options.fromString(filterName));

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