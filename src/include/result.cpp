#include "calyx.h"


using namespace calyx;


Result::Result(Expansion tree):
    _tree(tree)
{

}


const Expansion& 
Result::getTree()
{
    return _tree;
}

String_t 
Result::text(const Options& options)
{
    return _tree.flatten(options);
}