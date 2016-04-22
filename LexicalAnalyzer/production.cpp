#include "production.h"
#include <iostream>

production::production(psymbol lhs, vector<psymbol> rhs){
    this->lhs = lhs;
    this->rhs = rhs;
}

bool production::operator<(const production &rhs) const{
    return lhs.value < rhs.lhs.value;
}

void production::print_production(){
    cout << lhs.value << " " << "---> ";

    for(psymbol ps : rhs){
        cout << ps.value << " ";
    }
    cout << endl;
}
