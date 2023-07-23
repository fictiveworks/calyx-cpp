#include "atom_node.hpp"

using namespace calyx;

AtomNode::AtomNode(String_t atom)
    : _atom(atom)
{

}

std::optional<Expansion>
AtomNode::evaluate(Options& options, ErrorHolder& errors) const
{
    return Expansion(ATOM, _atom);
}

Production* 
AtomNode::deepcopy() const
{
    return new AtomNode(_atom);
}