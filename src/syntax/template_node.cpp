#include "template_node.hpp"

#include <regex>

using namespace calyx;

#define START_TOKEN '{'
#define END_TOKEN '}'

std::vector<std::string>
TemplateNode::fragmentString(const std::string& raw)
{
    std::vector<std::string> fragments;

    std::size_t lastExpressionPos = 0;
    std::size_t expressionPos = raw.find(START_TOKEN);
    while (expressionPos != std::string::npos)
    {
        std::size_t endExpressionPos = raw.find(END_TOKEN, expressionPos);
        if (endExpressionPos == std::string::npos)
        {
            break; // no closing bracket found, stop parsing
        }
        
        if (expressionPos > lastExpressionPos)
        {
            // add any atoms before the expression to the fragments
            fragments.push_back(raw.substr(lastExpressionPos, expressionPos - lastExpressionPos));
        }

        // add the expression to the fragments
        fragments.push_back(raw.substr(expressionPos, endExpressionPos - expressionPos + 1));

        lastExpressionPos = endExpressionPos + 1;

        // find next expression
        expressionPos = raw.find(START_TOKEN, lastExpressionPos);
    }

    // add remaining part if any
    if (lastExpressionPos < raw.size())
    {
        fragments.push_back(raw.substr(lastExpressionPos));
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