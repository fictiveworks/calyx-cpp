#include <catch2/catch_all.hpp>

#include <vector>
#include <string>
#include <include/calyx.h>
#include <syntax/template_node.hpp>

using namespace calyx;

TEST_CASE("Fragmenting tokens includes delimiters") 
{
    std::string raw = "{token} words {token}";
    std::vector<std::string> fragments = TemplateNode::fragmentString(raw);

    std::vector<std::string> expected = { "{token}", " words ", "{token}" };
    REQUIRE(expected == fragments);
}