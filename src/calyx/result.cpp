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
Result::getText(const StringConverter<String_t>& converter) const
{
    return _tree.flatten(converter);
}