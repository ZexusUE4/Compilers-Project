#include "semantic_functions.h"

map<string,sem_function> semantic_functions::sem_map { { "new_scope", &semantic_functions::new_scope}, {"exit_scope", &semantic_functions::exit_scope} ,
                        {"declare", &semantic_functions::declare}, };

void semantic_functions::default_call_back(psymbol* p){
//    cout << p->get_val() << ": ";
//
//    if(p->parent)
//        cout << "Parent: " << p->parent->get_val() << endl;
//    for(psymbol* child: p->childs){
//        cout << child->get_val() << " ";
//    }
//    cout << endl;
}

sem_function semantic_functions::get_call_back_of_name(string name)
{
    if(sem_map.find(name) != sem_map.end())
        return sem_map[name];
    else
        return &default_call_back;
}

/*---------------------------------------------------------------------------------*/

void semantic_functions::new_scope(psymbol* p){
    code_generator::sym_table.enter_scope();
}

void semantic_functions::exit_scope(psymbol* p){
    code_generator::sym_table.print();
    code_generator::sym_table.exit_scope();
}

/* # DECLARATION = PRIMITIVE_TYPE 'id' ';' {declare} */
void semantic_functions::declare(psymbol *p){

    psymbol* PRIMITIVE_TYPE = p->prev_sibling(2);
    psymbol* id = p->prev_sibling();

    entry* e = new entry(id->get_val(), PRIMITIVE_TYPE->get_lexeme_type());
    code_generator::sym_table.add(id->get_val(), e);
}
