#pragma once

#include <string>

using namespace std;

enum class psymbol_type {
    terminal,
    non_terminal,
    epsilon,
    or_operator
};

class psymbol
{
private:

public:

    /* string value of the production symbol */
    string value;

    /* type of the production symbol */
    psymbol_type type;

    /* production symbol constructor */
    psymbol(string val, psymbol_type typ);

    /* default constructor */
    psymbol();
};
