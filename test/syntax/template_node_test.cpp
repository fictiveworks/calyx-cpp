#include <catch2/catch_all.hpp>
#include <calyx/grammar.h>
#include <syntax/template_node.h>

using namespace calyx;

TEST_CASE("Template with no delimiters")
{
    Registry registry;
    ErrorHolder errs;
    Options& ops = registry.getOptions();
    String_t str = ops.fromString("One Two Three");
    
    std::optional<TemplateNode> node = TemplateNode::parse(str, registry, errs);
    REQUIRE(node.has_value());
    REQUIRE_FALSE(errs.hasError());

    std::optional<Expansion> exp = node->evaluate(registry, registry.getOptions(), errs);
    
    REQUIRE(exp.has_value());
    REQUIRE_FALSE(errs.hasError());

    REQUIRE(exp->getSymbol() == Exp::TEMPLATE);
    REQUIRE(exp->getTail()[0].getSymbol() == Exp::ATOM);
    REQUIRE(exp->getTail()[0].getTerm() == str);
}

TEST_CASE("Template with single expansion")
{
    Registry registry;
    ErrorHolder errs;
    Options& ops = registry.getOptions();
    
    registry.defineRule(ops.fromString("one"), {ops.fromString("ONE")}, errs);
    REQUIRE_FALSE(errs.hasError());
    
    std::optional<TemplateNode> node = TemplateNode::parse(ops.fromString("{one} two three"), registry, errs);
    REQUIRE(node.has_value());
    REQUIRE_FALSE(errs.hasError());

    std::optional<Expansion> exp = node->evaluate(registry, registry.getOptions(), errs);
    
    REQUIRE(exp.has_value());
    REQUIRE_FALSE(errs.hasError());

    REQUIRE(exp->getSymbol() == Exp::TEMPLATE);
    REQUIRE(exp->getTail()[0].getSymbol() == Exp::EXPRESSION);
    REQUIRE(exp->getTail()[0].getTail()[0].getSymbol() == Exp::UNIFORM_BRANCH);
    REQUIRE(exp->getTail()[0].getTail()[0].getTail()[0].getSymbol() == Exp::TEMPLATE);
    REQUIRE(exp->getTail()[0].getTail()[0].getTail()[0].getTail()[0].getSymbol() == Exp::ATOM);
    REQUIRE(exp->getTail()[0].getTail()[0].getTail()[0].getTail()[0].getTerm() == ops.fromString("ONE"));
    REQUIRE(exp->getTail()[1].getSymbol() == Exp::ATOM);
    REQUIRE(exp->getTail()[1].getTerm() == ops.fromString(" two three"));
}

TEST_CASE("Template with multiple expansions")
{
    Registry registry;
    ErrorHolder errs;
    Options& ops = registry.getOptions();
    
    registry.defineRule(ops.fromString("one"), {ops.fromString("ONE")}, errs);
    REQUIRE_FALSE(errs.hasError());
    registry.defineRule(ops.fromString("two"), {ops.fromString("TWO")}, errs);
    REQUIRE_FALSE(errs.hasError());
    
    std::optional<TemplateNode> node = TemplateNode::parse(ops.fromString("{one} {two} three"), registry, errs);
    REQUIRE(node.has_value());
    REQUIRE_FALSE(errs.hasError());

    std::optional<Expansion> exp = node->evaluate(registry, registry.getOptions(), errs);

    REQUIRE(exp.has_value());
    REQUIRE_FALSE(errs.hasError());
    
    REQUIRE(exp->getSymbol() == Exp::TEMPLATE);
    REQUIRE(exp->getTail()[0].getSymbol() == Exp::EXPRESSION);
    REQUIRE(exp->getTail()[0].getTail()[0].getSymbol() == Exp::UNIFORM_BRANCH);
    REQUIRE(exp->getTail()[0].getTail()[0].getTail()[0].getSymbol() == Exp::TEMPLATE);
    REQUIRE(exp->getTail()[0].getTail()[0].getTail()[0].getTail()[0].getSymbol() == Exp::ATOM);
    REQUIRE(exp->getTail()[0].getTail()[0].getTail()[0].getTail()[0].getTerm() == ops.fromString("ONE"));
    REQUIRE(exp->getTail()[1].getSymbol() == Exp::ATOM);
    REQUIRE(exp->getTail()[1].getTerm() == ops.fromString(" "));
    REQUIRE(exp->getTail()[2].getSymbol() == Exp::EXPRESSION);
    REQUIRE(exp->getTail()[2].getTail()[0].getSymbol() == Exp::UNIFORM_BRANCH);
    REQUIRE(exp->getTail()[2].getTail()[0].getTail()[0].getSymbol() == Exp::TEMPLATE);
    REQUIRE(exp->getTail()[2].getTail()[0].getTail()[0].getTail()[0].getSymbol() == Exp::ATOM);
    REQUIRE(exp->getTail()[2].getTail()[0].getTail()[0].getTail()[0].getTerm() == ops.fromString("TWO"));
    REQUIRE(exp->getTail()[3].getSymbol() == Exp::ATOM);
    REQUIRE(exp->getTail()[3].getTerm() == ops.fromString(" three"));
}

TEST_CASE("Template with single memo expansion")
{
    Registry registry;
    ErrorHolder errs;
    Options& ops = registry.getOptions();
    
    registry.defineRule(ops.fromString("one"), {ops.fromString("ONE"), ops.fromString("One"), ops.fromString("1")}, errs);
    REQUIRE_FALSE(errs.hasError());
    
    std::optional<TemplateNode> node = TemplateNode::parse(ops.fromString("{@one}{@one}{@one}"), registry, errs);
    REQUIRE(node.has_value());
    REQUIRE_FALSE(errs.hasError());

    std::optional<Expansion> exp = node->evaluate(registry, registry.getOptions(), errs);

    REQUIRE(exp.has_value());
    REQUIRE_FALSE(errs.hasError());
    
    REQUIRE(exp->getSymbol() == Exp::TEMPLATE);
    REQUIRE(exp->getTail()[0].getSymbol() == Exp::MEMO);
    REQUIRE(exp->getTail()[0].getTail()[0].getSymbol() == Exp::UNIFORM_BRANCH);
    REQUIRE(exp->getTail()[0].getTail()[0].getTail()[0].getSymbol() == Exp::TEMPLATE);
    REQUIRE(exp->getTail()[0].getTail()[0].getTail()[0].getTail()[0].getSymbol() == Exp::ATOM);
    REQUIRE(exp->getTail()[1].getSymbol() == Exp::MEMO);
    REQUIRE(exp->getTail()[1].getTail()[0].getSymbol() == Exp::UNIFORM_BRANCH);
    REQUIRE(exp->getTail()[1].getTail()[0].getTail()[0].getSymbol() == Exp::TEMPLATE);
    REQUIRE(exp->getTail()[1].getTail()[0].getTail()[0].getTail()[0].getSymbol() == Exp::ATOM);
    REQUIRE(exp->getTail()[2].getSymbol() == Exp::MEMO);
    REQUIRE(exp->getTail()[2].getTail()[0].getSymbol() == Exp::UNIFORM_BRANCH);
    REQUIRE(exp->getTail()[2].getTail()[0].getTail()[0].getSymbol() == Exp::TEMPLATE);
    REQUIRE(exp->getTail()[2].getTail()[0].getTail()[0].getTail()[0].getSymbol() == Exp::ATOM);

    String_t firstTerm = exp->getTail()[0].getTail()[0].getTail()[0].getTail()[0].getTerm();
    String_t secondTerm = exp->getTail()[1].getTail()[0].getTail()[0].getTail()[0].getTerm();
    String_t thirdTerm = exp->getTail()[2].getTail()[0].getTail()[0].getTail()[0].getTerm();
    
    REQUIRE(firstTerm == thirdTerm);
    REQUIRE(secondTerm == thirdTerm);
}