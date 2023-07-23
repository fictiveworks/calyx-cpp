#include "expression_node.hpp"

using namespace calyx;

#define MEMO_SIGIL '@'
#define UNIQUE_SIGIL '$'

ExpressionNode::ExpressionNode(const String_t reference, const Registry& registry)
    : _reference(reference), _registry(registry)
{

}

std::shared_ptr<ExpressionNode>
ExpressionNode::parse(const String_t raw, const Registry& registry)
{
    return std::make_shared<ExpressionNode>(raw, registry);
}

Expansion
ExpressionNode::evaluate(Options& options) const
{
    ErrorHolder errs;
    std::optional<Rule> rule = _registry.expand(_reference, errs);

    // FIXME: error handling for evaluation
    Expansion eval = rule->evaluate(options);

    return Expansion(EXPRESSION, std::move(eval.getTail()));
}

Production*
ExpressionNode::deepcopy() const
{
    return new ExpressionNode(_reference, _registry);
}