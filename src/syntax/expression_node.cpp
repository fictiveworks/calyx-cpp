#include "expression_node.h"

#include "memo_node.h"
#include "unique_node.h"


using namespace calyx;

#define MEMO_SIGIL '@'
#define UNIQUE_SIGIL '$'

ExpressionNode::ExpressionNode(String_t reference)
    : _reference(reference)
{

}

std::shared_ptr<Production>
ExpressionNode::parse(const String_t raw, const Registry& registry, ErrorHolder& errors)
{
    if (raw[0] == MEMO_SIGIL)
    {
        Options& ops = registry.getOptions();
        std::string rawString = ops.toString(raw).substr(1);
        return std::make_shared<MemoNode>(MemoNode(ops.fromString(rawString)));
    }
    else if (raw[0] == UNIQUE_SIGIL)
    {
        Options& ops = registry.getOptions();
        std::string rawString = ops.toString(raw).substr(1);
        return std::make_shared<UniqueNode>(UniqueNode(ops.fromString(rawString)));
    }
    
    
    return std::make_shared<ExpressionNode>(ExpressionNode(raw));
}

std::optional<Expansion>
ExpressionNode::evaluate(
    Registry& registry,
    Options& options,
    ErrorHolder& errors
) const
{
    ErrorHolder errs;
    std::shared_ptr<Rule> rule = registry.expand(_reference, errs);

    if (!rule || errs.hasError())
    {
        return {};
    }

    std::optional<Expansion> eval = rule->evaluate(registry, options, errors);

    if (!eval || errs.hasError())
    {
        return {};
    }

    return Expansion(Exp::EXPRESSION, *eval);
}