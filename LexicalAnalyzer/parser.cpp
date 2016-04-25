#include "parser.h"
#include "production_reader.h"
#include <iostream>

parser::parser(string productions_file_name){
    productions = production_reader::get_instance()->read(productions_file_name);

    solve_oring();
    print_productions();
    solve_first_sets();
    print_first_sets();
}

void parser::print_productions()
{
    for(production p : productions){
        p.print_production();
    }
}

void parser::solve_oring()
{
    set<production> new_prods ;//will remove this later

    for(production p : productions){
        psymbol lhs = p.get_lhs();
        vector<psymbol> rhs = p.get_rhs();
        vector<psymbol> new_rhs ;

        for(int i = 0 ; i < rhs.size() ; ++i){
            if( rhs[i].get_type() == psymbol_type::or_operator ){
                new_prods.insert(production(lhs,new_rhs));
                productions_graph[lhs].push_back(new_rhs);
                new_rhs.clear();
            }
            else{
                new_rhs.push_back(rhs[i]);
            }
        }

        if( new_rhs.size() ){
            new_prods.insert(production(lhs,new_rhs));
            productions_graph[lhs].push_back(new_rhs);
        }
    }

    productions = new_prods ;
}

bool parser::is_visited( psymbol ps )
{
    return visited.count(ps) != 0 ;
}

bool parser::is_nullable( psymbol ps )
{
    switch( ps.get_type() ){
        case psymbol_type::terminal :{
            return false ;
        }
        case psymbol_type::epsilon :{
            return true ;
        }
        case psymbol_type::non_terminal :{
            return nullable.count(ps) != 0 ;
        }
    }
}

void parser::solve_first_sets()
{
    map<psymbol,vector<vector<psymbol>>> ::iterator it = productions_graph.begin();
    while( it != productions_graph.end() ){
        if( !is_visited(it->first) ){
            solve_first_symbol(it->first);
        }
        ++it ;
    }
}

void parser::solve_first_symbol( psymbol left )
{
    visited.insert(left);
    vector<vector<psymbol>>& adj = productions_graph[left] ;
    bool global_nullable_test = 0 ;

    for(int i = 0 ; i < adj.size() ; ++i){
        bool solved = 0 ;
        bool local_nullable_test = 1 ;//in case all are nullable non terminal

        for(int j = 0 ; !solved && j < adj[i].size() ; ++j){
            psymbol ps = adj[i][j] ;
            switch( ps.get_type() ){
                case psymbol_type::terminal :{//base case
                    solved = 1 ;
                    local_nullable_test = 0 ;
                    first_sets[left].insert(make_pair(ps,production(left,adj[i])));
                    break ;
                }
                case psymbol_type::epsilon :{
                    local_nullable_test = 1 ;
                    solved = 1 ;
                    break ;
                }
                case psymbol_type::non_terminal :{
                    if( !is_visited(ps) )
                        solve_first_symbol(ps);//recurse //ONLY non-valid if we have left recursion

                    set<pair<psymbol,production>> temp = first_sets[ps] ;
                    set<pair<psymbol,production>>::iterator it = temp.begin();
                    while(it!=temp.end()){
                        if( it->first.get_type() == psymbol_type::terminal ){
                            first_sets[left].insert(make_pair(it->first,production(left,adj[i])));
                        }
                        ++it ;
                    }
                    if( !is_nullable(ps) ){
                        solved = 1 ;
                        local_nullable_test = 0 ;
                    }
                    break ;
                }
            }//END SWITCH

        }//END NEST FOR //nullable solved = 0

        if( local_nullable_test ){
            global_nullable_test = 1 ;
        }
    }//END MAIN FOR

    if( global_nullable_test ){
        nullable.insert(left);
        psymbol eps = psymbol("\\L",psymbol_type::epsilon) ;
        vector<psymbol> rhs ;
        rhs.push_back(eps);
        production left_to_eps = production(left,rhs);
        first_sets[left].insert(make_pair(eps,left_to_eps));
    }

}//END FUNCTION

void parser::print_first_sets()
{
    cout<<"First sets :-"<<endl;

    map<psymbol,set<pair<psymbol,production>>>::iterator it = first_sets.begin();

    while(it != first_sets.end()){
        cout<<it->first.get_val()<<" ";
        set<pair<psymbol,production>> temp = it->second ;
        set<pair<psymbol,production>>::iterator itt = temp.begin();

        cout<<"{ ";
        while( itt != temp.end() ){
            if( itt != temp.begin() )cout<<" , ";
            cout<<"( "<<itt->first.get_val()<<" , ";
            production t = itt->second ;
            cout<<t.to_string()<<")";
            ++itt ;
        }
        cout<<" }"<<endl;

        ++it ;
    }

    cout<<endl;
}







