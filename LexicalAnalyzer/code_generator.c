
void code_generator::register_call_back(int stack_size, psymbol* node){
    call_back_map[stack_size] = node;
}

void code_generator::check_for_call_back(int cur_stack_size)
{
    if(call_back_map.find(cur_stack_size) != call_back_map.end()){

    }
}
