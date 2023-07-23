#pragma once

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

        Expansion evaluate(Options& options) const override;

    protected:

        Production* deepcopy() const override;

    private:

        const String_t _atom;

    };
}