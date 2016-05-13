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

public:

    static sem_function get_call_back_of_name(string name);


    /* semantic functions */
    static void default_call_back(psymbol* p);

    static void new_scope(psymbol *p);
    static void exit_scope(psymbol *p);
    static void declare(psymbol *p);

};
