#include "template_node.h"

#include <regex>
#include <vector>
#include <sstream>
#include <string>

#include "atom_node.h"
#include "expression_chain.h"
#include "expression_node.h"

using namespace calyx;

#define START_TOKEN '{'
#define END_TOKEN '}'
#define DEREF_TOKEN '.'


std::vector<std::string>
split(const std::string& s, const char delim)
{
    // from https://stackoverflow.com/questions/14265581/parse-split-a-string-in-c-using-string-delimiter-standard-c
    std::vector<std::string> result;
    std::stringstream ss(s);
    std::string item;

    while (std::getline(ss, item, delim))
    {
        result.push_back(item);
    }

    return result;
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
    std::string rawString = ops.toString(raw);
    std::vector<std::string> fragments = fragmentString(rawString);

    std::vector<std::shared_ptr<Production>> concatNodes;

    for (auto atom : fragments)
    {
        if (atom.empty())
        {
            continue;
        }
        String_t convertedAtom = ops.fromString(atom);

        if (atom.starts_with(START_TOKEN) && atom.ends_with(END_TOKEN))
        {
            // remove the braces
            std::string expression = atom.substr(1, atom.size() - 2);

            std::vector<std::string> components = split(expression, DEREF_TOKEN);

            if (components.size() > 1)
            {
                std::shared_ptr<Production> prod = std::make_shared<ExpressionChain>(std::move(components));
                concatNodes.push_back(prod);
            }
            else
            {
                std::shared_ptr<Production> prod = ExpressionNode::parse(ops.fromString(expression), registry, errors);

                if (prod == nullptr || errors.hasError())
                {
                    return {};
                }

                concatNodes.push_back(prod);
            }
        }
        else
        {
            std::shared_ptr<AtomNode> prod = std::make_shared<AtomNode>(convertedAtom);
            concatNodes.push_back(prod);
        }
    }

    return TemplateNode(concatNodes);
}


std::optional<Expansion>
TemplateNode::evaluate(
    Registry& registry,
    Options& options,
    ErrorHolder& errors
) const
{
    std::vector<Expansion> evaluatedResults;

    for (const auto& node : _concatNodes)
    {
        std::optional<Expansion> exp = node->evaluate(registry, options, errors);

        if (!exp)
        {
            return {};
        }

        evaluatedResults.push_back(*exp);
    }

    Expansion exp = Expansion(
        Exp::TEMPLATE,
        evaluatedResults
    );

    return exp;
}