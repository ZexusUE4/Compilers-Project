#include "parser.h"
#include "production_reader.h"
#include <iostream>

parser::parser(string productions_file_name){
    productions = production_reader::get_instance()->read(productions_file_name);
    solve_oring();

    //Bolbol's section

    make_first_follow();
}

void parser::make_first_follow()
{
    solve_first_sets();
    solve_follow_sets();

    print_productions();
    cout<<endl;
    print_first_sets();
    cout<<endl;
    print_components();
    cout<<endl;
    print_follow_sets();
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
                    solved = 1 ;///HERE
                    break ;
                }
                case psymbol_type::non_terminal :{
                    if( !is_visited(ps) )
                        solve_first_symbol(ps);//recurse //ONLY non-valid if we have left recursion

                    set<pair<psymbol,production>> temp = first_sets[ps] ;
                    set<pair<psymbol,production>>::iterator it = temp.begin();
                    while(it!=temp.end()){
                        if( it->first.get_type() == psymbol_type::terminal ){//First(A) = First(B) - eps
                            first_sets[left].insert(make_pair(it->first,production(left,adj[i])));
                        }
                        ++it ;
                    }
                    if( !is_nullable(ps) ){//IF B is not nullable , we're done
                        solved = 1 ;
                        local_nullable_test = 0 ;
                    }
                    break ;
                }
            }//END SWITCH

        }//END NEST FOR

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

set<pair<psymbol,production>> parser::get_first_sets( psymbol ps )
{
    return first_sets[ps] ;
}

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
}

void parser::create_follow_graphs()
{
    for( production p : productions ){
        psymbol lhs = p.get_lhs() ;
        vector<psymbol> rhs = p.get_rhs();

        if( rhs.size() == 1 && rhs[0].get_type() == psymbol_type::epsilon ){// A -> eps
            continue ;
        }

        for(int i = rhs.size()-1 ; i >= 0 ; --i){
            if( rhs[i].get_type() == psymbol_type::non_terminal ){
                follow_graph[rhs[i]].insert(lhs);//Non-Terminal -> LHS
                follow_graph_trans[lhs].insert(rhs[i]);//LHS -> Non-Terminal
                if( !is_nullable(rhs[i]) ){
                    break ;
                }
            }
            else{
                break ;
            }
        }
    }
}

void parser::create_components()
{
    visited.clear();
    for(production p : productions){
        psymbol lhs = p.get_lhs();
        if( !is_visited(lhs) )
            scc_pass1(lhs);
    }

    visited.clear();
    while( follow_toplogical.size() ){
        psymbol ps = follow_toplogical.top(); follow_toplogical.pop();
        if( !is_visited(ps) ){
            scc_pass2(ps);
            ++components_cnt ;
        }
    }
    visited.clear();/* To free un needed memory */

    map<psymbol,set<psymbol>>::iterator it = follow_graph.begin();
    while(it != follow_graph.end()){
        int from = component[it->first];
        for( psymbol ps : it->second ){
            int to = component[ps] ;
            if( from != to )components_graph[from].insert(to);
        }
        ++it ;
    }

}

void parser::print_components()
{
    cout<<"Strongly connected components :-"<<endl;
    map<psymbol,int>::iterator it = component.begin();

    map<int,set<psymbol>> temp ;

    while(it!=component.end()){
        temp[it->second].insert(it->first);
        ++it ;
    }

    map<int,set<psymbol>>::iterator ix = temp.begin();
    while( ix != temp.end() ){
        cout<<ix->first<<" { ";

        for( psymbol s : ix->second ){
            cout<<s.get_val()<<" ";
        }

        cout<<"}"<<endl;
        ++ix ;
    }

    cout<<"The SCC graph :-"<<endl;
    map<int,set<int>>::iterator itt = components_graph.begin();
    while(itt!=components_graph.end()){
        cout<<itt->first<<" -> ";

        for( int to : itt->second )cout<<to<<" ";

        cout<<endl;
        ++itt ;
    }
}

void parser::scc_pass1( psymbol s )
{
    visited.insert(s);

    for( psymbol to : follow_graph[s] ){
        if( !is_visited(to) ){
            scc_pass1(to);
        }
    }

    follow_toplogical.push(s);
}

void parser::scc_pass2( psymbol s )
{
    visited.insert(s);
    component[s] = components_cnt ;

    for( psymbol to : follow_graph_trans[s] ){
        if( !is_visited(to) ){
            scc_pass2(to);
        }
    }
}

vector<psymbol> parser::get_first_sets_symbols( psymbol ps )
{
    vector<psymbol> ret ;
    if( ps.get_type() == psymbol_type::terminal || ps.get_type() == psymbol_type::epsilon ){
        ret.push_back(ps);
    }
    else{
        set<pair<psymbol,production>> temp = first_sets[ps];
        set<pair<psymbol,production>>::iterator it = temp.begin();
        while( it!=temp.end() ){
            ret.push_back( it->first );
            ++it ;
        }
    }

    return ret ;
}

void parser::add_to_follow_first( psymbol to , psymbol from )
{
    vector<psymbol> first_set = get_first_sets_symbols(from);
    for( psymbol ps : first_set ){
        if(ps.get_type() != psymbol_type::epsilon)
            follow_sets[component[to]].insert(ps);//Follow(to) += ( First(from) - eps )
    }
}

void parser::add_to_follow_follow( int to , int from )//Edited //Comments only
{
    set<psymbol> my_set = follow_sets[from];
    for( psymbol ps : my_set ){
        follow_sets[to].insert(ps);//Follow(to) += Follow(from)
    }
}

void parser::solve_follow_sets()
{
    create_follow_graphs();
    create_components();

    psymbol dummy = psymbol("$",psymbol_type::start_dummy);//marks EOF
    psymbol start_symbol = production_reader::get_instance()->get_first_symbol();

    follow_sets[component[start_symbol]].insert(dummy);// Add $ to Follow(S)

    //1) add first of my neighbor , until non-nullable
    for( production p : productions ){
        vector<psymbol> rhs = p.get_rhs();
        for(int i = 0 ; i < rhs.size() ; ++i){
            if( rhs[i].get_type() == psymbol_type::non_terminal ){
                for(int j = i+1 ; j < rhs.size() ; ++j){//BUG
                    add_to_follow_first(rhs[i],rhs[j]);
                    if( !is_nullable(rhs[j]) ){
                        break ;
                    }
                }
            }
        }
    }

    //by default , circles are now solved due to SCC

    //2) add non-circles , dfs on graph
    visited_components = vector<bool>(components_cnt,0);
    for( int i = 0 ; i < components_cnt ; ++i ){
        if(!visited_components[i]){
            dfs_follow(i);
        }
    }

    follow_graph.clear();
    follow_graph_trans.clear();
    visited.clear();
    visited_components.clear();
}

void parser::dfs_follow( int node )
{
    visited_components[node] = true ;

    set<int> adj ;

    if( components_graph.count(node) )
        adj = components_graph[node];
    else
        return ;

    for( int to : adj ){
        if(!visited_components[to]){
            dfs_follow(to);
        }
        add_to_follow_follow(node,to);
    }
}

set<psymbol> parser::get_follow_symbols( psymbol ps )
{
    return follow_sets[component[ps]] ;
}

void parser::print_follow_sets()
{
    cout<<"Follow sets :-"<<endl;

    set<psymbol> printed ;

    for(production p : productions){
        psymbol lhs = p.get_lhs();

        if( printed.count(lhs) )continue ;
        printed.insert(lhs);
        cout<<"Follow("<<lhs.get_val()<<") : { ";
        for( psymbol ps : get_follow_symbols(lhs) ){
            cout<<ps.get_val()<<" ";
        }
        cout<<"}";
        cout<<endl;
    }
}










