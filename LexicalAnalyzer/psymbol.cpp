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
