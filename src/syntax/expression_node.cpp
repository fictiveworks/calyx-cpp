#include "expression_node.hpp"

using namespace calyx;

#define MEMO_SIGIL '@'
#define UNIQUE_SIGIL '$'

ExpressionNode::ExpressionNode(const String_t reference)
    : _reference(reference)
{

}

std::optional<ExpressionNode>
ExpressionNode::parse(const String_t raw, const Registry& registry, ErrorHolder& errors)
{
    return ExpressionNode(raw);
}

std::optional<Expansion>
ExpressionNode::evaluate(
    Registry& registry,
    Options& options,
    ErrorHolder& errors
) const
{
    ErrorHolder errs;
    std::optional<Rule> rule = registry.expand(_reference, errs);

    if (!rule || errs.hasError())
    {
        return {};
    }

    std::optional<Expansion> eval = rule->evaluate(registry, options, errors);

    if (!eval || errs.hasError())
    {
        return {};
    }

    return Expansion(EXPRESSION, *eval);
}