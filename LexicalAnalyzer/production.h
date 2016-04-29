#pragma once

#include "psymbol.h"
#include <vector>

class production
{
private:

public:
    /* production constructor */
    production(psymbol lhs, vector<psymbol> rhs);

    /* default constructor */
    production();

    /* Operator < overloading */
	bool operator<(const production &b) const;

    /* prints the production */
    void print_production();
    string to_string();

    /* left hand side of the production, must be a non-terminal */
    psymbol lhs;

    /* right hand side of the production */
    vector<psymbol> rhs;

    vector<psymbol> get_rhs() const ;
    psymbol get_lhs() const ;


};
