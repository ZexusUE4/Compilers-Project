#include "production.h"
#include <set>
#include <map>
#include <stack>

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
    vector<bool> visited_components ;

    /* For any symbol , get set of first symbols , each coupled with the pointer to production extracted from */
    map<psymbol,set<pair<psymbol,production>>> first_sets ;

    map<int,set<psymbol>> follow_sets ;
    map<psymbol,int> component ;
    stack<psymbol> follow_toplogical ;
    map<psymbol,set<psymbol>> follow_graph , follow_graph_trans ;
    map<int,set<int>> components_graph ;

    void solve_oring();                           /* Turns A -> B | C into A -> B , A -> C */
    bool is_nullable( psymbol ps );               /* Checks if any symbol is null */
    void print_productions();
    bool is_visited( psymbol ps );
    void make_first_follow();

    /* Related to first Sets calculation */
    void solve_first_sets();
    void solve_first_symbol( psymbol left );
    void print_first_sets();
    set<pair<psymbol,production>> get_first_sets( psymbol ps );

    /* Relates to follow sets calculation */
    int components_cnt = 0 ;
    void solve_follow_sets();
    void create_follow_graphs();
    void create_components();
    void scc_pass1( psymbol s );
    void scc_pass2( psymbol s );
    void print_components();
    vector<psymbol> get_first_sets_symbols( psymbol ps );
    void add_to_follow_first( psymbol to , psymbol from );
    void add_to_follow_follow( int to , int from );
    void dfs_follow( int node );
    void make_graph( psymbol ps );
    set<psymbol> get_follow_symbols( psymbol ps );
    void print_follow_sets();

public:

    /* parser constructor */
    parser(string productions_file_name);

//    creat_first_sets()
};
