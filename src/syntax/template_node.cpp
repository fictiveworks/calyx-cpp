#include "template_node.hpp"

#include <regex>

using namespace calyx;

static std::vector<std::string> fragmentString(const std::string& raw)
{
    static const std::string expression = "\\{[A-Za-z0-9_@$<>.]+\\}";
    static const std::string expressionRegex = "(?=" + expression + ")|(?<=" + expression + ")";
    static const std::regex regex(expressionRegex);

    std::sregex_token_iterator it{ raw.begin(), raw.end(), regex, -1 };

    std::vector<std::string> fragments{ it, {} };

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