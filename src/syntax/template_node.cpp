#include "template_node.hpp"

#include <regex>

using namespace calyx;

void resetFragment(std::vector<std::string>& fragments, std::string& fragment)
{
    
}

std::vector<std::string>
TemplateNode::fragmentString(const std::string& raw)
{
    static const char startToken = '{';
    static const char endToken = '}';

    std::vector<std::string> fragments;

    std::string fragment = "";

    for (char chr : raw)
    {
        if (chr == raw.back() || chr == startToken)
        {
            if (fragment.size() > 0)
            {
                fragments.push_back(fragment);
            }
            fragment = "";
        }
        
        fragment += chr;

        if (chr == endToken && fragments.size() > 0)
        {
            fragments.back() += chr;
            fragment = "";
        }
    }

    return fragments;
}

TemplateNode::TemplateNode(std::vector<std::shared_ptr<Production>> concatNodes)
    : _concatNodes(concatNodes)
{

}

TemplateNode TemplateNode::parse(const String_t& raw, const Registry& registry)
{
    static const std::string startToken = "{";
    static const std::string endToken = "}";

    const Options& ops = registry.getOptions();
    std::string rawString = ops._converter.toString(raw);
    std::vector<std::string> fragments = fragmentString(rawString);

    std::vector<std::shared_ptr<Production>> concatNodes;

    for (const auto& atom : fragments)
    {
        if (atom.size() == 0)
        {
            continue;
        }

        if (atom.starts_with(startToken) && atom.ends_with(endToken))
        {
            std::string expression = atom.substr(1, atom.size() - 1);


        }
    }

}


Expansion TemplateNode::evaluate(Options& options) const
{
    std::vector<std::unique_ptr<Expansion>> evaluatedResults;

    for (const auto& node : _concatNodes)
    {
        evaluatedResults.push_back(std::make_unique<Expansion>(node->evaluate(options)));
    }

    Expansion exp = Expansion(
        TEMPLATE,
        std::move(evaluatedResults)
    );

    return exp;
}



Production* TemplateNode::deepcopy() const
{
    return nullptr;
}