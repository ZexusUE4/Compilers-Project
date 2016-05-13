#include "code_generator.h"

symbol_table code_generator::sym_table;

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
