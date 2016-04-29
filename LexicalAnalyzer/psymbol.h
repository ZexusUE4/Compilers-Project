#pragma once

#include <string>

using namespace std;

enum class psymbol_type {
    terminal,
    non_terminal,
    epsilon,
    or_operator,
    start_dummy,
    synch
};

class psymbol
{
private:
    /* string value of the production symbol */
    string value;

    /* type of the production symbol */
    psymbol_type type;

public:
    /* production symbol constructor */
    psymbol(string val, psymbol_type typ);

    /* default constructor */
    psymbol();

    string get_val() const ;
    psymbol_type get_type() const ;

    bool operator<(const psymbol &b) const;
};
