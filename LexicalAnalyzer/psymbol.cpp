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
