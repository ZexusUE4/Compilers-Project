#pragma once

#include "psymbol.h"
#include "symbol_table.h"
#include <stack>

class code_generator
{
private:
    map<int,stack<psymbol*> > call_back_map;

public:

    static symbol_table sym_table;

    void register_call_back(int stack_size, psymbol* node);

    void check_for_call_backs(int cur_stack_size);
};
