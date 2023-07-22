#include "atom_node.hpp"

using namespace calyx;

AtomNode::AtomNode(String_t atom)
    : _atom(atom)
{

}

Expansion
AtomNode::evaluate(Options& options) const
{
    return Expansion(ATOM, _atom);
}