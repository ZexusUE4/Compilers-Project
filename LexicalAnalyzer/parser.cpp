#include "parser.h"
#include "production_reader.h"
#include <iostream>

parser::parser(string productions_file_name){
    productions = production_reader::get_instance()->read(productions_file_name);
    solve_oring();

    eliminate_LR();
    left_factoring();
    productions = adjust_production_set();
    make_first_follow();

    create_table();

}
void parser::create_table()
{
    for(auto it : first_sets)
    {
        psymbol left = it.first;
        for(auto it2 : it.second )
        {
            psymbol tran = it2.first;
            if(tran.get_type() == psymbol_type::epsilon)
                continue;
            production next = it2.second;
            table[make_pair(left,tran)] = next;
        }
        set<psymbol> follow = get_follow_symbols(left);
        production left_to_eps = production(left,vector<psymbol>(1,psymbol("\\L",psymbol_type::epsilon)));
        psymbol temp = psymbol("synch",psymbol_type::synch);
        production synch = production(temp,vector<psymbol>());
        bool flag = is_nullable(left);
        for(auto it2: follow)
        {
            psymbol tran = it2;
            if(flag && table.find(make_pair(left,tran))!= table.end())
                has_error = true;
            if(flag)
                table[make_pair(left,tran)] = left_to_eps;
            else if(table.find(make_pair(left,tran))== table.end())
                table[make_pair(left,tran)] = synch;
        }
    }
}
void parser::derive (string token_type , string token_val, ofstream &out)
{
    if(symbols.empty())
    {
        out << derived+ "       /* token "+ token_val + " is discarded */";
        return;
    }
    bool matched = false;
    ptr2 = ptr1;
    while (!matched&& !symbols.empty())
    {
        if(ptr2 <= ptr1)
        {
            ptr2 = ptr1 ;
            while(ptr2 < derived.size() && derived[ptr2]!=' ')
                ptr2++;
        }
        psymbol cur = symbols.top();
        if(cur.get_type() == psymbol_type::terminal || cur.get_type() == psymbol_type::start_dummy)
        {
            if(cur.get_val() == token_type || (cur.get_type() == psymbol_type::start_dummy && token_type =="END_OF_FILE"))
            {
                matched = true;
                out <<  "/* Matched "+ cur.get_val() + " with "+ (cur.get_type() == psymbol_type::start_dummy ? "$":token_val) + " */" <<endl;
            }
            else
                out << "/* Error missing "+ cur.get_val() + " ,inserted */"<<endl;
            ptr1 = ptr2+1;
            symbols.pop();
        }
        else
        {
            psymbol temp = psymbol((token_type=="END_OF_FILE" ? "$": "\'"+token_type+"\'"),(token_type=="END_OF_FILE" ? psymbol_type::start_dummy : psymbol_type::terminal));
            if(table.find(make_pair(cur,temp)) != table.end())
            {
                production prod = table[make_pair(cur,temp)];
                symbols.pop();
                if(prod.lhs.get_type() == psymbol_type::synch)
                {
                    ptr1 = ptr2+1;
                    out << "/* Synchronisation happened here due to illegal production for "+ cur.get_val() + " */" <<endl;
                }
                else
                {
                    if(prod.rhs.size() == 1 && prod.rhs[0].get_val() == "\\L")
                    {
                        derived = derived.substr(0,ptr1) + (ptr2+1 >= derived.size()?"" :derived.substr(ptr2+1,derived.size()-ptr2-1));
                        out <<derived << endl;
                        ptr2 =ptr1;
                        continue;
                    }

                    for(int i = prod.rhs.size()-1 ; i>=0;i--)
                        symbols.push(prod.rhs[i]);
                    string der = "";
                    for(psymbol symbol: prod.rhs)
                        der+=symbol.get_val() +" ";
                    derived = derived.substr(0,ptr1) + der +(ptr2+1 >= derived.size()?"" :derived.substr(ptr2+1,derived.size()-ptr2-1));
                    out << derived <<endl;
                    ptr2 = ptr1;
                }
            }
            else
            {
                out <<"/* Error:(illegal production " << cur.get_val() <<") - discard "+ token_val << " */"<<endl;
                matched = true;
            }
        }
    }
}
void parser::start(ofstream &out)
{
    while(!symbols.empty())
        symbols.pop();
    psymbol start_symbol = production_reader::get_instance()->get_first_symbol();
    symbols.push(psymbol("$",psymbol_type::start_dummy));
    symbols.push(start_symbol);
    derived = start_symbol.get_val();
    ptr1 = 0;
    ptr2 = derived.size()-1;
    out << derived << endl;

    print_parse_table();
    return;
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

void parser::add_to_follow_follow( int to , int from )////Edited //Comments only
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

void parser::print_parse_table(){
    set<psymbol> non_terminals;
    set<psymbol> terminals;

    for(auto it = table.begin();it != table.end(); it++){
        terminals.insert(it->first.first);
        non_terminals.insert(it->first.second);
    }
    fstream file;
    file.open("parse_table.html", ifstream::out);

    file << "<!DOCTYPE html><html><body><table style=\"width:100%\" border=\"3\">";

    file << "<thead>";
    file << "<th bgcolor=\"##6699ff\"> Production </th>";
    for(psymbol p : non_terminals){
        file << "<th bgcolor=\"##6699ff\">" << p.get_val() << "</th>";
    }
    file << "</thead>";

    for(psymbol p: terminals){
        file << "<tr><td align=\"center\" bgcolor=\"##6699ff\">" << p.get_val() << "</td>";
        for(psymbol n: non_terminals){
            production prod = table[make_pair(p,n)];
            file << "<td align=\"center\">" << prod.to_html_string() << "</td>";
        }
        file << "</tr>";
    }
}

set<production> parser::adjust_production_set()
{
    set<production> ret;
    for(pair<psymbol, vector<vector<psymbol>>> e : productions_graph)
    {
        for(int i= 0; i< e.second.size(); i++)
        {
            production p= production(e.first, e.second[i]);
            ret.insert(p);
        }
    }
    return ret;
}

void parser::eliminate_immediate_LR(psymbol lhs)
{
    psymbol lhs_LR= psymbol(lhs.get_val() + "\'", lhs.get_type());

    vector<vector<psymbol>> rhs= productions_graph[lhs];
    vector<vector<psymbol>> new_rhs;
    vector<vector<psymbol>> new_rhs_LR;
    for(int i= 0; i< rhs.size(); i++)
    {
        if(rhs[i][0].get_val() != lhs.get_val())
        {
            vector<psymbol> curr_beta;
            for(int j= 0; j< rhs[i].size(); j++)
            {
                if(rhs[i][j].get_type() == psymbol_type::epsilon) continue;
                curr_beta.push_back(rhs[i][j]);
            }
            if(curr_beta.empty()) continue;
            curr_beta.push_back(lhs_LR);
            new_rhs.push_back(curr_beta);
        }
        else
        {
            vector<psymbol> curr_alpha;
            for(int j= 1; j< rhs[i].size(); j++){
                if(rhs[i][j].get_type() == psymbol_type::epsilon) continue;
                curr_alpha.push_back(rhs[i][j]);
            }
            if(curr_alpha.empty()) continue;
            curr_alpha.push_back(lhs_LR);
            new_rhs_LR.push_back(curr_alpha);
        }
    }
    if(new_rhs_LR.size())
    {
        psymbol eps = psymbol("\\L",psymbol_type::epsilon);
        vector<psymbol> last;
        last.push_back(eps);
        new_rhs_LR.push_back(last);
        productions_graph[lhs]= new_rhs;
        productions_graph[lhs_LR]= new_rhs_LR;
    }
}

void parser::eliminate_LR()
{
    set<psymbol> visited_productions;
    int counter= 0;
    for(pair<psymbol, vector<vector<psymbol>>> e : productions_graph)
    {
        bool is_recursive= true;
        int counter= 0;
        while(is_recursive)
        {
            is_recursive= false;
            for(int i= 0; i< e.second.size(); i++)
            {
                if(visited_productions.find(e.second[i][0]) != visited_productions.end())
                {

                    vector<vector<psymbol>> found_production= productions_graph[e.second[i][0]];
                    vector<psymbol> top;
                    for(int j= 0; j< found_production.size(); j++)
                    {
                        vector<psymbol> curr_production;
                        for(int k= 0; k< found_production[j].size(); k++)
                        {
                            if(found_production[j][k].get_type() == psymbol_type::epsilon) continue;
                            curr_production.push_back(found_production[j][k]);
                        }
                        for(int k= 1; k< e.second[i].size(); k++)
                        {
                            if(e.second[i][k].get_type()== psymbol_type::epsilon) continue;
                            curr_production.push_back(e.second[i][k]);
                        }
                        if(curr_production.empty())
                            curr_production.push_back(psymbol("\\L", psymbol_type::epsilon));

                        if(j == 0) top= curr_production;
                        else e.second.push_back(curr_production);
                        is_recursive= true;
                    }
                    e.second[i]= top;
                }
            }
        }
        visited_productions.insert(e.first);
        productions_graph[e.first]= e.second;
        eliminate_immediate_LR(e.first);
    }
}

void parser::left_factoring()
{
    psymbol new_prod;
    map<psymbol, vector<vector<psymbol>>> tmp= productions_graph;
    bool rerun= true;
    while(rerun){
        rerun= false;
        for(pair<psymbol, vector<vector<psymbol>>> e : tmp)
        {
            int counter= 0;
            do
            {
                counter++;
                productions_graph[e.first]= e.second;
                new_prod= psymbol(e.first.get_val() + dot_creator(counter), psymbol_type::non_terminal);
            }while(get_longest_common_prefix(e.second, new_prod));
            rerun |= (counter>1);
        }
        tmp= productions_graph;
    }
}

string parser::dot_creator(int num)
{
    string str= "";
    for(int i= 0; i< num; i++) str+= '.';
    return str;
}

bool parser::get_longest_common_prefix(vector<vector<psymbol>> &v_v, psymbol new_prod)
{
    bool is_longest= false;
    int ind= 0;
    vector<int> curr_prefix;
    vector<int> vec= match_first_letter(v_v);

    int counter= 0;
    while(1)
    {
        if(vec.empty()) break;
        is_longest= false;
        for(int i= 0; i< vec.size(); i++)
        {
            for(int j= i+1; j< vec.size(); j++)
            {
                if(ind==v_v[vec[i]].size() || v_v[vec[i]][ind].get_val()!=v_v[vec[j]][ind].get_val())
                {
                    is_longest= true;
                    break;
                }
            }
            if(is_longest) break;
        }
        if(is_longest) break;
        ind++;
    }

    if(is_longest)
    {
        remove_common_factor(ind, vec, v_v, new_prod);
    }
    return is_longest;
}

void parser::remove_common_factor(int prefix_sz, vector<int> const &vec,
                                  vector<vector<psymbol>> &v_v, psymbol new_prod)
{
    vector<psymbol> common_prefix;
    for(int i= 0; i< prefix_sz; i++)
    {
        common_prefix.push_back(v_v[vec[0]][i]);
    }
    common_prefix.push_back(new_prod);

    vector<vector<psymbol>> new_prod_v_v;
    for(int i= 0; i< vec.size(); i++)
    {
        vector<psymbol> curr;
        if(prefix_sz == v_v[vec[i]].size()) curr.push_back(psymbol("\\L", psymbol_type::epsilon));
        for(int j= prefix_sz; j< v_v[vec[i]].size(); j++)
        {
            curr.push_back(v_v[vec[i]][j]);
        }
        new_prod_v_v.push_back(curr);
    }

    vector<vector<psymbol>> tmp;
    int ind= 0;
    for(int i= 0; i< v_v.size(); i++)
    {
        if(i == vec[ind])
        {
            ind++;
            continue;
        }
        tmp.push_back(v_v[i]);
    }

    v_v= tmp;
    v_v.push_back(common_prefix);

    productions_graph[new_prod]= new_prod_v_v;
}

vector<int> parser::match_first_letter(vector<vector<psymbol>> const &v_v)
{
    vector<int> ret;
    for(int i= 0; i< v_v.size(); i++)
    {
        ret.push_back(i);
        for(int j= i+1; j< v_v.size(); j++)
        {
            if(v_v[i][0].get_val() == v_v[j][0].get_val()) ret.push_back(j);
        }
        if(ret.size() > 1) return ret;
        else ret.clear();
    }
    return ret;
}


