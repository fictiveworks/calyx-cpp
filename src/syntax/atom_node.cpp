#include "atom_node.h"

using namespace calyx;

AtomNode::AtomNode(String_t atom)
    : _atom(atom)
{

}

std::optional<Expansion>
AtomNode::evaluate(
    Registry& registry,
    Options& options,
    ErrorHolder& errors
) const
{
    return Expansion(Exp::ATOM, _atom);
}