#include "production.h"
#include <iostream>

production::production(psymbol lhs, vector<psymbol> rhs){
    this->lhs = lhs;
    this->rhs = rhs;
}
production::production()
{

}

bool production::operator<(const production & b) const{
    if( lhs.get_val() == b.lhs.get_val() ){
        return rhs < b.rhs ;
    }
    return lhs< b.lhs ;
}

void production::print_production(){
    cout << lhs.get_val() << " " << "---> ";

    for(psymbol ps : rhs){
        cout << ps.get_val() << " ";
    }
    cout << endl;
}

string production::to_string()
{
    string ret = lhs.get_val() ;
    ret += " ---> " ;

    for(psymbol ps : rhs){
        ret += ps.get_val();
        ret += " ";
    }

    return ret ;
}

string production::to_html_string(){
    if(!lhs.get_val().size()){
        return "ERROR";
    }
    else if(lhs.get_type() == psymbol_type::synch){
        return "SYNCH";
    }
    else{
        string ret = lhs.get_val() ;
        ret += " &#x2192 ";
        for(psymbol ps : rhs){
            ret += ps.get_html_val();
            ret += " ";
        }
        return ret;
    }
}
vector<psymbol> production::get_rhs() const
{
    return rhs ;
}

psymbol production::get_lhs() const
{
    return lhs ;
}
