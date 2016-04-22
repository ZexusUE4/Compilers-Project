#pragma once

#include "psymbol.h"
#include <vector>

class production
{
private:

public:

    /* left hand side of the production, must be a non-terminal */
    psymbol lhs;

    /* right hand side of the production */
    vector<psymbol> rhs;

    /* production constructor */
    production(psymbol lhs, vector<psymbol> rhs);

    /* Operator < overloading */
	bool operator<(const production &rhs) const;

    /* prints the production */
    void print_production();
};
