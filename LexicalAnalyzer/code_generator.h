#pragma once

#include "psymbol.h"
#include "symbol_table.h"
#include <stack>

class code_generator
{
private:
    map<int,stack<psymbol*> > call_back_map;

    code_generator();
    static code_generator* _instance;

    int err_cnt;
public:

    symbol_table sym_table;

    static code_generator* instance();
    void register_call_back(int stack_size, psymbol* node);

    void check_for_call_backs(int cur_stack_size);

    void throw_error(string err_msg, string action);
};
