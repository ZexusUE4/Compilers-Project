#include "production.h"
#include <set>
#include <map>
#include <iostream>

class parser
{

private:

    /* a set containing all productions found in the productions file */
    set<production> productions;

    /* A -> { aa , bb , cc } instead of A -> aa | bb | cc */
    map<psymbol,vector<vector<psymbol>>> productions_graph ;

    /* Contains only non-terminals */
    set<psymbol> nullable ; // Don't use it in practice

    set<psymbol> visited ;

    /* For any symbol , get set of first symbols , each coupled with the pointer to production extracted from */
    map<psymbol,set<pair<psymbol,production>>> first_sets ;

    map<psymbol,set<psymbol>> follow_sets ;


    void solve_oring();
    bool is_nullable( psymbol ps );
    void print_productions();
    void solve_first_sets();
    void solve_first_symbol( psymbol left );
    void print_first_sets();
    bool is_visited( psymbol ps );


public:

    /* parser constructor */
    parser(string productions_file_name);

//    creat_first_sets()
};
