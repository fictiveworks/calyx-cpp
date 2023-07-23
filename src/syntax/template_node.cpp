#include "template_node.hpp"

#include <regex>
#include <vector>
#include <sstream>
#include <string>

#include "atom_node.hpp"
#include "expression_node.hpp"

using namespace calyx;

#define START_TOKEN '{'
#define END_TOKEN '}'
#define DEREF_TOKEN '.'


std::vector<std::string>
split(const std::string& s, const char delim) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delim)) {
        tokens.push_back(token);
    }
    return tokens;
}

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

std::optional<TemplateNode>
TemplateNode::parse(const String_t& raw, const Registry& registry, ErrorHolder& errors)
{
    const Options& ops = registry.getOptions();
    std::string rawString = ops._converter.toString(raw);
    std::vector<std::string> fragments = fragmentString(rawString);

    std::vector<std::shared_ptr<Production>> concatNodes;

    for (auto atom : fragments)
    {
        if (atom.size() == 0)
        {
            continue;
        }
        String_t convertedAtom = ops._converter.fromString(atom);

        if (atom.starts_with(START_TOKEN) && atom.ends_with(END_TOKEN))
        {
            // remove the braces
            std::string expression = atom.substr(1, atom.size() - 1);

            std::vector<std::string> components = split(expression, DEREF_TOKEN);

            if (components.size() > 1)
            {
                // TODO: expr chains
            }
            else
            {
                auto prod = ExpressionNode::parse(convertedAtom, registry, errors);

                if (!prod) 
                {
                    return {};
                }

                concatNodes.push_back(std::make_shared<ExpressionNode>(*prod));
            }
        }
        else
        {
            std::shared_ptr<AtomNode> prod = std::make_shared<AtomNode>(convertedAtom);
            concatNodes.push_back(prod);
        }

        return TemplateNode(concatNodes);
    }

}


std::optional<Expansion>
TemplateNode::evaluate(Options& options, ErrorHolder& errors) const
{
    std::vector<std::unique_ptr<Expansion>> evaluatedResults;

    for (const auto& node : _concatNodes)
    {
        std::optional<Expansion> exp = node->evaluate(options, errors);

        if (!exp)
        {
            return {};
        }

        evaluatedResults.push_back(std::make_unique<Expansion>(*exp));
    }

    Expansion exp = Expansion(
        TEMPLATE,
        std::move(evaluatedResults)
    );

    return exp;
}



Production* TemplateNode::deepcopy() const
{
    return new TemplateNode(_concatNodes);
}