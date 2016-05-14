#include "semantic_functions.h"
#include <cstdlib>

#define sym_table code_generator::instance()->sym_table

code_generator* semantic_functions::cg = code_generator::instance();

map<string,sem_function> semantic_functions::sem_map { { "new_scope", &semantic_functions::new_scope}, {"exit_scope", &semantic_functions::exit_scope} ,
                        {"declare", &semantic_functions::declare}, {"assignment", &semantic_functions::assignment}, { "set_type_float", &semantic_functions::set_type_float},
                        {"set_type_int", &semantic_functions::set_type_int}, {"if_statement", &semantic_functions::if_statement}, {"else_statement", &semantic_functions::else_statement},
                        {"if_else_end", &semantic_functions::if_else_end}, {"pass_type_next", &semantic_functions::pass_type_next},{"exp_typ2", &semantic_functions::exp_typ2},
                         {"exp_typ3", &semantic_functions::exp_typ3}, {"set_type_id", &semantic_functions::set_type_id}, {"set_type_id_pass", &semantic_functions::set_type_id_pass},
                         {"term_end", &semantic_functions::term_end}, {"term_float",&semantic_functions::term_float}, {"term_int", &semantic_functions::term_int},
                         {"term_pass_next", &semantic_functions::term_pass_next}, {"term_max_type" , &semantic_functions::term_max_type},{"simple_add",&semantic_functions::simple_add},
                         {"check_id_declared", &semantic_functions::check_id_declared}, {"simple_sign_neg", &semantic_functions::simple_sign_neg}, { "factor_exp_type" , &semantic_functions::factor_exp_type},
                         {"set_next_addr", &semantic_functions::set_next_addr}, {"set_loop_begin", &semantic_functions::set_loop_begin}, {"while_end", &semantic_functions::while_end},
                        };


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

vector<int>* semantic_functions::mergeLists(vector<int>* v1, vector<int>* v2)
{
	vector<int>* ret = new vector<int>();
	ret->insert(ret->end(),v2->begin(),v2->end());
	ret->insert(ret->end(),v1->begin(),v1->end());
	return ret;
}

vector<int>* semantic_functions::makeList(int next)
{
	vector<int> *v = new vector<int>();
	v->push_back(next);
	return v;
}


/*---------------------------------------------------------------------------------*/

void semantic_functions::new_scope(psymbol* p){
    sym_table.enter_scope();
}

void semantic_functions::exit_scope(psymbol* p){
    //sym_table.print();
    //sym_table.exit_scope();
}

/* # DECLARATION = PRIMITIVE_TYPE 'id' ';' {declare} */
void semantic_functions::declare(psymbol *p){

    psymbol* PRIMITIVE_TYPE = p->prev_sibling(2);
    psymbol* id = p->prev_sibling();

    entry* e = new entry(id->get_val(), PRIMITIVE_TYPE->get_lexeme_type());
    if(sym_table.exists_last_scope(id->get_val())){
        cg->throw_error("Dublicate declaration of \"" + id->get_val() + "\"","Skipped declaration.");
    }
    else
    {
        sym_table.add(id->get_val(), e);
        cg->declare_symbol(e->get_symbol_pointer(),e->get_type());
    }
    p->parent->data.next_addr = cg->get_program_counter();
}

/* # ASSIGNMENT = 'id' {check_id_declared} 'assign' EXPRESSION ';' {assignment} */
void semantic_functions::assignment(psymbol *p){
    psymbol* id = p->prev_sibling(3);
    psymbol* EXPRESSION = p->prev_sibling();
    cg->assign_symbol(id->get_val(),EXPRESSION->data.type);
    p->parent->data.next_addr = cg->get_program_counter();
}
void semantic_functions::check_id_declared(psymbol* p)
{

    if(!sym_table.exists(p->get_val())){
        cg->throw_error("Undeclared variable used \""+p->get_val() + "\"" ,"Fatal Error ");
    }
}
/* # PRIMITIVE_TYPE = 'float' {set_type_float} */
void semantic_functions::set_type_float(psymbol* p)
{
    p->data.type = "float";
}

/* # PRIMITIVE_TYPE = 'int' {set_type_int} */
void semantic_functions::set_type_int(psymbol* p)
{
    p->data.type = "int";
}

/* # IF = 'if' '(' EXPRESSION ')' '{' {new_scope} STATEMENT {if_statement} '}' {exit_scope} 'else' '{' {new_scope} STATEMENT {else_statement} '}' {if_else_end} */
void semantic_functions::if_statement(psymbol *p)
{
    cg->add_instruction("goto ");
    p->data.next_addr = cg->get_program_counter();
}

void semantic_functions::else_statement(psymbol *p)
{
    p->data.next_addr = cg->get_program_counter();
    exit_scope(p);
}

void semantic_functions::if_else_end(psymbol* p)
{
    psymbol* EXPRESSION = p->prev_sibling(8);
    psymbol* IF_STATEMENT = p->prev_sibling(5);
    psymbol* ELSE_STATEMENT = p->prev_sibling();

    if(EXPRESSION->data.type == "bool")
    {
        cg->back_patch(EXPRESSION->data.falseList, IF_STATEMENT->data.next_addr);
        cg->back_patch(IF_STATEMENT->data.next_addr-1, ELSE_STATEMENT->data.next_addr);
    }
    else
    {
        cg->throw_error("Expression is not of type: bool ","Flow of control error !");
    }
}

/* # EXPRESSION = SIMPLE_EXPRESSION {pass_type_next} EXPRESSION. {exp_typ2} */
void semantic_functions::pass_type_next(psymbol* p)
{
    psymbol* EXPRESSION = p->next_sibling();
    EXPRESSION->data.type = p->data.type;
    p->data.next_addr = cg->get_program_counter();
}

void semantic_functions::exp_typ2(psymbol* p)
{
    psymbol* parent = p->parent;
    parent->data.type = p->data.type;
    parent->data.trueList = p->data.trueList;
    parent->data.falseList = p->data.falseList;
    parent->data.next_addr = cg->get_program_counter();
}

/* # EXPRESSION. = 'relop' SIMPLE_EXPRESSION {exp_typ3} */
void semantic_functions::exp_typ3(psymbol* p)
{
    psymbol* relop = p->prev_sibling();
    string type = cg->solve_casting(p->parent->data.type,p->data.type);

    if(type == "int"){
        cg->add_instruction(cg->get_comp_inst(relop->get_val(),"int") + " ");
    }
    else{
        cg->add_instruction("fcmpl");
        cg->add_instruction(cg->get_comp_inst(relop->get_val(),"float") + " ");
    }

    p->parent->data.type = "bool";

    p->parent->data.falseList = makeList(cg->get_program_counter() - 1);
    p->parent->data.trueList = new vector<int>();

    p->parent->data.next_addr = cg->get_program_counter();
}

/* # FACTOR = 'id' {set_type_id} */
void semantic_functions::set_type_id(psymbol* p)
{
    if(sym_table.exists_last_scope(p->get_val())){
        entry* e = sym_table.get_entry(p->get_val());
        p->parent->data.type = e->get_type();
    }
    else
    {
        cg->throw_error("Undeclared variable used \""+p->get_val() + "\"" ,"Fatal Error ");
    }
}

/* # FACTOR = '(' EXPRESSION ')' {factor_exp_type} */
void semantic_functions::factor_exp_type(psymbol* p)
{
    psymbol* EXP = p->prev_sibling();
    p->parent->data.type = EXP->data.type;
    p->parent->data.next_addr = cg->get_program_counter();
}

/* # TERM = 'id' {set_type_id_pass} TERM' {term_end} */

void semantic_functions::set_type_id_pass(psymbol* p)
{
    psymbol* term_d = p->next_sibling();

    if(sym_table.exists(p->get_val())){
        entry* e = sym_table.get_entry(p->get_val());
        term_d->data.type = e->get_type();
        cg->insert_id_to_stack(p->get_val());
        p->parent->data.next_addr = cg->get_program_counter();
    }
    else
    {
        cg->throw_error("Undeclared variable used \""+p->get_val() + "\"" ,"Fatal ErrorZZZZ ");
    }
}

void semantic_functions::term_end(psymbol* p)
{
    p->parent->data.type = p->data.type;
    p->parent->data.next_addr = cg->get_program_counter();
}

/* # TERM = 'float_v' {term_float} TERM' {term_end}*/
void semantic_functions::term_float(psymbol *p)
{
    cg->insert_float_const_to_stack(atof(p->get_val().c_str()));
     psymbol* term_d = p->next_sibling();
     term_d->data.type = "float";
     p->parent->data.next_addr = cg->get_program_counter();
}

/* # TERM = 'int_v' {term_int} TERM' {term_end} */
void semantic_functions::term_int(psymbol *p)
{
     cg->insert_int_const_to_stack(atoi(p->get_val().c_str()));
     psymbol* term_d = p->next_sibling();
     term_d->data.type = "int";
     p->parent->data.next_addr = cg->get_program_counter();
}

/* # TERM = '(' EXPRESSION {term_pass_next} ')' TERM' {term_end} */
void semantic_functions::term_pass_next(psymbol*p)
{
    psymbol* term_d = p->next_sibling(2);
    term_d->data.type = p->data.type;
    p->parent->data.next_addr = cg->get_program_counter();
}

/* # TERM' = 'mulop' FACTOR {term_max_type} TERM' {term_end} */
void semantic_functions::term_max_type(psymbol* p)
{
    psymbol* mul = p->prev_sibling();
    if(mul->get_val() == "*")
        cg->binary_arth(p->parent->data.type,p->data.type,op_type::MUL);
    else
        cg->binary_arth(p->parent->data.type,p->data.type,op_type::DIV);

    psymbol* term_d = p->next_sibling();
    term_d->data.type = cg->max_type(p->parent->data.type,p->data.type);
    p->parent->data.next_addr = cg->get_program_counter();
}

/* # SIMPLE_EXPRESSION = '-' TERM {simple_sign_pos} SIMPLE_EXPRESSION' {term_end} */
void semantic_functions::simple_sign_neg(psymbol *p)
{
    cg->neg(p->data.type);
    psymbol* SIMPLE = p->next_sibling();
    SIMPLE->data.type = p->data.type;
    p->parent->data.next_addr = cg->get_program_counter();
}

/* # SIMPLE_EXPRESSION' = 'addop' TERM {simple_add} SIMPLE_EXPRESSION' {term_end} */
void semantic_functions::simple_add(psymbol* p)
{
    psymbol* addop = p->prev_sibling();
    psymbol* SIMPLE = p->next_sibling();

    if(addop->get_val() == "+")
        cg->binary_arth(p->parent->data.type, p->data.type,op_type::ADD);
    else
        cg->binary_arth(p->parent->data.type, p->data.type,op_type::SUB);

    SIMPLE->data.type = cg->max_type(p->parent->data.type,p->data.type);
    p->parent->data.next_addr = cg->get_program_counter();
}

void semantic_functions::set_next_addr(psymbol* p)
{
    p->parent->data.next_addr = cg->get_program_counter();
}

/* # WHILE = 'while' {set_loop_begin} '(' EXPRESSION ')' '{' STATEMENT '}' {while_end} */
void semantic_functions::set_loop_begin(psymbol* p)
{
    p->data.next_addr = cg->get_program_counter();
}

void semantic_functions::while_end(psymbol* p)
{
    psymbol* EXP = p->prev_sibling(4);
    psymbol* STATEMENT = p->prev_sibling();
    psymbol* WHILE = p->prev_sibling(6);

    if(EXP->data.type == "bool")
    {
        cg->add_instruction("goto L" + cg->int_to_string(WHILE->data.next_addr));
        STATEMENT->data.next_addr = cg->get_program_counter();
        cg->back_patch(EXP->data.falseList,STATEMENT->data.next_addr);
    }
    else
    {
        cg->throw_error("Expression is not of type: bool ","Flow of control error !");
    }
    exit_scope(p);
}
