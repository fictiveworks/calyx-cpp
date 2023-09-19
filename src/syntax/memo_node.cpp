#include "memo_node.h"

#include "../registry.h"
using namespace calyx;

MemoNode::MemoNode(String_t symbol):
    _symbol(std::move(symbol))
{
}

std::optional<Expansion>
MemoNode::evaluate(Registry& registry, Options& options, ErrorHolder& errors) const
{
    std::shared_ptr<Expansion> exp = registry.memoizeExpansion(_symbol, errors);

    if (exp == nullptr || errors.hasError())
    {
        return {};
    }
    
    return Expansion(Exp::MEMO, *exp);
}