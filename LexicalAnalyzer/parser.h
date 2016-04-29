#include "production.h"
#include <set>
#include <map>
#include <stack>
#include <fstream>

class parser
{

private:

    /* a set containing all productions found in the productions file */
    set<production> productions;

    /* A -> { aa , bb , cc } instead of A -> aa | bb | cc */
    map<psymbol,vector<vector<psymbol>>> productions_graph ;

    /* Contains only non-terminals , returns if A -> eps */
    set<psymbol> nullable ; // Don't use it in practice

    /* Related to internal SCC processing */
    set<psymbol> visited ;
    vector<bool> visited_components ;

    /* For any symbol , get set of first symbols , each coupled with the pointer to production extracted from */
    map<psymbol,set<pair<psymbol,production>>> first_sets ;

    /* Related to first & follow sets processing , Don't use any of them */
    map<int,set<psymbol>> follow_sets ;
    map<psymbol,int> component ;
    stack<psymbol> follow_toplogical ;
    map<psymbol,set<psymbol>> follow_graph , follow_graph_trans ;
    map<int,set<int>> components_graph ;

    map<pair<psymbol,psymbol> , production > table;/* table used for parsing */

    stack<psymbol> symbols;/* stack to be used in parsing */

    string derived; /* to be returned to be printed */

    int ptr1 = 0,ptr2=0; /* pointer to the string to be derived in the next iteration */

    /* General Methods */
    void solve_oring();                           /* Turns A -> B | C into A -> B , A -> C */
    bool is_nullable( psymbol ps );               /* Checks if any symbol is null */
    void make_first_follow();                     /* Main method to calculate first & Follow , Call it after LR & LF */
    void print_productions();                     /* Print set<production> productions */
    void print_first_sets();                      /* Prints first set of all Non-terminals , each coupled with its
                                                                            related production  */
    void print_components();                      /* Prints equivalent Non-terminals , in terms of follow sets */
    void print_follow_sets();                     /* Prints follows set of all Non-terminals */

    /* Getters , to be used by developers */
    set<pair<psymbol,production>> get_first_sets( psymbol ps );
    vector<psymbol> get_first_sets_symbols( psymbol ps );
    set<psymbol> get_follow_symbols( psymbol ps );

    /* Related to first Sets calculation */
    void solve_first_sets();
    void solve_first_symbol( psymbol left );

    /* Relates to follow sets calculation */
    int components_cnt = 0 ;
    void solve_follow_sets();
    void create_follow_graphs();                  /* if A -> aB , then we have edge from B to A */
    void create_components();                     /* Calls SCC methods and creates the SCC graph */
    void scc_pass1( psymbol s );                  /* SCC Algorithm on Non-terminal graph pass1 */
    void scc_pass2( psymbol s );                  /* SCC Algorithm on Non-terminal graph pass2 */
    void add_to_follow_first( psymbol to , psymbol from );
                                                  /* Follow(to) += ( First(from) - eps ) */
    void add_to_follow_follow( int to , int from );
                                                  /* Follow(to) += Follow(from) */
    void dfs_follow( int node );                  /* DFS on the SCC Graph to solve Follow(A) += Follow(B) */
    bool is_visited( psymbol ps );                /* Related to internal calculation of SCC */
    void make_graph( psymbol ps );
    void create_table();/* create table to be used in parsing latter */

public:

    /* parser constructor */
    parser(string productions_file_name);

    void start(ofstream &out);
    void derive(string token_type,string token_val, ofstream &out);
    bool has_error;

    /* Prints the parse table into an html file */
    void print_parse_table();
//    creat_first_sets()
};
