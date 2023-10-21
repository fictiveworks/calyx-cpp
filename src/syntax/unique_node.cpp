#include "unique_node.h"

#include "../registry.h"

using namespace calyx;

UniqueNode::UniqueNode(String_t symbol):
    _symbol(std::move(symbol))
{
}

std::optional<Expansion> UniqueNode::evaluate(Registry& registry, Options& options, ErrorHolder& errors) const
{
    std::optional<Expansion> exp = registry.uniqueExpansion(_symbol, errors);

    if (!exp || errors.hasError())
    {
        return {};
    }

    return Expansion(Exp::UNIQ, *exp);
}
