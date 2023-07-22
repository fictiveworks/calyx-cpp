#include <catch2/catch_all.hpp>

#include <include/calyx.h>
#include <expansion.hpp>
#include <vector>
#include <memory>
#include <options.hpp>

using namespace calyx;

TEST_CASE("Flatten expansion to atoms")
{

    std::vector<std::unique_ptr<Expansion>> tail;
    tail.push_back(std::make_unique<Expansion>(ATOM, "-ONE-"));
    tail.push_back(std::make_unique<Expansion>(ATOM, "-TWO-"));
    tail.push_back(std::make_unique<Expansion>(ATOM, "-THREE-"));

    auto exp = Expansion(TEMPLATE, std::move(tail));

    auto ops = Options();
    String_t atoms = exp.flatten(ops);

    REQUIRE("-ONE--TWO--THREE-" == atoms);
}
