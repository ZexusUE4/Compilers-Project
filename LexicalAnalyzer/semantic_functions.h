#pragma once

#include<iostream>
#include "psymbol.h"
#include "code_generator.h"
#include "token.h"
#include <map>

typedef void (*sem_function)(psymbol*);
class semantic_functions
{
private:
    static map<string,sem_function> sem_map;
    static code_generator* cg;
public:

    static sem_function get_call_back_of_name(string name);
    static vector<int>* mergeLists(vector<int>* v1, vector<int>* v2);
    static vector<int>* makeList(int next);

    /* semantic functions */
    static void default_call_back(psymbol* p);

    static void new_scope(psymbol *p);
    static void exit_scope(psymbol *p);
    static void declare(psymbol *p);
    static void assignment(psymbol*);
    static void check_id_declared(psymbol* p);
    static void set_type_float(psymbol* p);
    static void set_type_int(psymbol* p);
    static void if_statement(psymbol *p);
    static void else_statement(psymbol *p);
    static void if_else_end(psymbol* p);
    static void pass_type_next(psymbol* p);
    static void exp_typ2(psymbol* p);
    static void exp_typ3(psymbol* p);
    static void set_type_id(psymbol* p);
    static void set_type_id_pass(psymbol* p);
    static void term_end(psymbol* p);
    static void term_float(psymbol *p);
    static void term_int(psymbol *p);
    static void term_pass_next(psymbol*p);
    static void term_max_type(psymbol* p);
    static void simple_sign_neg(psymbol *p);
    static void simple_add(psymbol* p);
    static void factor_exp_type(psymbol* p);
    static void set_next_addr(psymbol* p);
    static void set_loop_begin(psymbol* p);
    static void while_end(psymbol* p);
};
