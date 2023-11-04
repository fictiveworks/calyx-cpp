#include "grammar.h"


using namespace calyx;


Result::Result(Expansion tree):
    _tree(tree)
{

}


const Expansion& 
Result::getTree() const
{
    return _tree;
}

String_t 
Result::getText(const Options& options) const
{
    return _tree.flatten(options);
}