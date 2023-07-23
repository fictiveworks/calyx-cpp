#include <catch2/catch_all.hpp>

#include <vector>
#include <string>
#include <include/calyx.h>
#include <syntax/template_node.hpp>

using namespace calyx;

/**
 * @brief Run a fragmentation test. Fragments the {@code raw} input into a list of tokens and asserts that it is equal to
 * the {@code expected} token list
 *
 * @param raw The raw input to fragmentize
 * @param expected The expected fragmentation output
 */
void testFragmentation(const std::string&& raw, const std::vector<std::string> expected)
{
    std::vector<std::string> fragments = TemplateNode::fragmentString(raw);
    REQUIRE(expected == fragments);
}

TEST_CASE("Fragmenting tokens")
{
    SECTION("Fragmenting tokens includes expressions and atoms")
    {
        testFragmentation("{expression} atom {expression}", { "{expression}", " atom ", "{expression}" });
    }

    SECTION("Fragmenting tokens starting with atoms")
    {
        testFragmentation("start atom {expression} atom {expression}", { "start atom ", "{expression}", " atom ", "{expression}" });
    }

    SECTION("Fragmenting tokens two space-separated expressions")
    {
        testFragmentation("{expression} {expression}", { "{expression}", " ", "{expression}" });
    }

    SECTION("Fragmenting tokens two space-separated expressions")
    {
        testFragmentation("{expression} {expression}", { "{expression}", " ", "{expression}" });
    }

    SECTION("Fragmenting tokens just expressions")
    {
        testFragmentation("{expression1}{expression2}", { "{expression1}", "{expression2}" });
    }

    SECTION("Fragmenting tokens just atoms")
    {
        testFragmentation("this is just a single atom", { "this is just a single atom" });
    }

    SECTION("Fragmenting tokens empty input")
    {
        testFragmentation("", { });
    }
}





