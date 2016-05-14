#pragma once

#include "psymbol.h"
#include "symbol_table.h"
#include <fstream>
#include <stack>

enum class relop_type
{
    GT,LT,GE,LE,EQ,NQ
};
enum class op_type
{
    ADD, SUB, MUL, DIV
};

class code_generator
{
private:
    map<int,stack<psymbol*> > call_back_map;

    code_generator();
    static code_generator* _instance;
    ofstream out1;
    ofstream out2;
    vector<string> lines;

    int err_cnt;
public:

    symbol_table sym_table;
    static code_generator* instance();
    void register_call_back(int stack_size, psymbol* node);

    void check_for_call_backs(int cur_stack_size);

    void throw_error(string err_msg, string action);

    void add_instruction(string inst);

    void back_patch(vector<int>* keys, int line_no);

    void back_patch(int key, int line_no);
    void print_start_template(ofstream&);
    void print_end_template(ofstream&);
    void printing_template(ofstream& output, int index, string type);

    int string_to_int(string str, int index);

    void commit_code();

    int get_program_counter();

    void binary_arth( string type_1 , string type_2 , op_type op );

    string int_to_string(int);

    string float_to_string(float);

    string get_comp_inst(string relop,string data_type);

    relop_type get_relop_type(string relop_type);

    void neg( string type );

    string arth_op( op_type op , string type );

    void add_inst_optimized( string line );

    void insert_id_to_stack( string id_lexeme );

    void insert_int_const_to_stack( int val );

    void insert_float_const_to_stack( float val );

    void assign_symbol( string id , string type_of_expression );

    void declare_symbol( int table_pointer , string type );

    string max_type(string type1, string type2);

    string solve_casting( string type_1 , string type_2 );
};
