#include "code_generator.h"
#include <iostream>

code_generator* code_generator::_instance = NULL;

code_generator::code_generator()
{
    err_cnt = 0;
}

code_generator* code_generator::instance()
{
    if(_instance == NULL)
    {
        _instance = new code_generator();
    }
    return _instance;
}

void code_generator::register_call_back(int stack_size, psymbol* node){
    call_back_map[stack_size].push(node);
}

void code_generator::check_for_call_backs(int cur_stack_size)
{
    if(call_back_map.find(cur_stack_size) != call_back_map.end()){
        while(call_back_map[cur_stack_size].size()){
            psymbol* node = call_back_map[cur_stack_size].top();
            node->call_back(node);
            call_back_map[cur_stack_size].pop();
        }
    }
};

void code_generator::throw_error(string err_msg, string action)
{
    cout << "Error no.: " << (++err_cnt) << "\t";
    cout << "Msg: " + err_msg << "\t";
    cout << "Action: " + action << endl;
}
