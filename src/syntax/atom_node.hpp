#pragma once

#include <optional>
#include "../registry.hpp"
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

        std::optional<Expansion> evaluate(
            Registry& registry,
            Options& options,
            ErrorHolder& errors
        ) const override;

    private:

        const String_t _atom;

    };
}