
#include "psymbol.h"
#include "semantic_functions.h"

psymbol::psymbol(string val,string lexeme_type, psymbol_type typ){
    value = val;
    type = typ;
    this->lexeme_type = lexeme_type;

    if(type == psymbol_type::terminal){
        value = value.substr(1,value.size()-2);
    }

    this-> call_back = semantic_functions::get_call_back_of_name("default");
    this-> pre_call = semantic_functions::get_call_back_of_name("default");

    this->parent = NULL;
}

psymbol::psymbol(string val, psymbol_type typ){
    value = val;
    type = typ;

    if(type == psymbol_type::terminal){
        value = value.substr(1,value.size()-2);
    }

    this-> call_back = semantic_functions::get_call_back_of_name("default");
    this-> pre_call = semantic_functions::get_call_back_of_name("default");

    this->parent = NULL;
}

psymbol::psymbol(){

}

string psymbol::get_lexeme_type() const
{
    return lexeme_type;
}

void psymbol::set_lexeme_type(string type)
{
    lexeme_type = type;
}

string psymbol::get_val() const
{
    return value ;
}

string psymbol::get_html_val() const
{
    if(value == "\\L"){
        string eps;
        eps += "&#8712;";
        return eps;
    }
    return value ;
}

psymbol_type psymbol::get_type() const
{
    return type ;
}

bool psymbol::operator<(const psymbol &b) const
{
    return get_val() < b.get_val();
}

psymbol* psymbol::next_sibling(int distance)
{
    if(this->parent == NULL)
    {
        return NULL;
    }
    vector<psymbol*> p_childs = this->parent->childs;
    int i = 0;
    while(p_childs[i] != this){
        i++;
    }

    cout << this->parent->get_val() << " " << p_childs.size() << endl;
    while(distance){
        i--;
        distance--;
    }

    if(i >= 0)
        return p_childs[i];
    else
        return NULL;
}

psymbol* psymbol::prev_sibling(int distance)
{
    if(this->parent == NULL)
    {
        return NULL;
    }
    vector<psymbol*> p_childs = this->parent->childs;
    int i = 0;
    while(p_childs[i] != this){
        i++;
    }
    while(distance){
        i++;
        distance--;
    }

    if(i < p_childs.size())
        return p_childs[i];
    else
        return NULL;
}

void psymbol::set_val(string new_val){
    value = new_val;
}
