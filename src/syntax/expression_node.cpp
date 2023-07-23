#include "expression_node.hpp"

using namespace calyx;

#define MEMO_SIGIL '@'
#define UNIQUE_SIGIL '$'

ExpressionNode::ExpressionNode(const String_t reference, const Registry& registry)
    : _reference(reference), _registry(registry)
{

}

std::optional<std::shared_ptr<ExpressionNode>>
ExpressionNode::parse(const String_t raw, const Registry& registry, ErrorHolder& errors)
{
    return std::make_shared<ExpressionNode>(raw, registry);
}

std::optional<Expansion>
ExpressionNode::evaluate(Options& options, ErrorHolder& errors) const
{
    ErrorHolder errs;
    std::optional<Rule> rule = _registry.expand(_reference, errs);

    if (!rule)
    {
        return {};
    }

    std::optional<Expansion> eval = rule->evaluate(options, errors);

    if (!eval)
    {
        return {};
    }

    return Expansion(EXPRESSION, std::make_unique<Expansion>(*eval));
}

Production*
ExpressionNode::deepcopy() const
{
    return new ExpressionNode(_reference, _registry);
}