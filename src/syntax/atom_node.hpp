#pragma once

#include <optional>
#include "../production.hpp"
#include "../expansion.hpp"
#include "../string_converter.hpp"
#include "../options.hpp"

namespace calyx
{

    class AtomNode : public Production
    {
    public:

        AtomNode(String_t atom);

        std::optional<Expansion> evaluate(Options& options, ErrorHolder& errors) const override;

    protected:

        Production* deepcopy() const override;

    private:

        const String_t _atom;

    };
}