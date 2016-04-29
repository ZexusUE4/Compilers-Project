#include "psymbol.h"

psymbol::psymbol(string val,psymbol_type typ){
    value = val;
    type = typ;

    if(type == psymbol_type::terminal){
        value = value.substr(1,value.size()-2);
    }
}

psymbol::psymbol(){

}

string psymbol::get_val() const
{
    if(value == "\\L"){
        string eps;
        eps += (char)238;
        return eps;
    }
    return value ;
}

string psymbol::get_html_val() const
{
    if(value == "\\L"){
        string eps;
        eps += "&#8712;";
        return eps;
    }
    return value ;
}

psymbol_type psymbol::get_type() const
{
    return type ;
}

bool psymbol::operator<(const psymbol &b) const
{
    return get_val() < b.get_val();
}
