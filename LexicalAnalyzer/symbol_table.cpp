#include "symbol_table.h"
#include <iostream>

symbol_table::symbol_table()
{
    table_counter = 0;
}

symbol_table::~symbol_table()
{
    table.clear();
}

void symbol_table::add( string lexeme , entry* value )
{
    value->set_symbol_pointer(++table_counter);
    table[table.size()-1][lexeme] = value;
}

entry* symbol_table::get_entry( string lexeme )
{
    for(int i = table.size()-1 ; i >= 0 ; --i ){
        if(table[i].find(lexeme) != table[i].end())
            return table[i][lexeme];
    }
    return NULL;
}

int symbol_table::get_symbol_pointer(string lexeme)
{
    entry* e = get_entry(lexeme);
    if(e)
        return e->get_symbol_pointer();
    else
        return -1;
}

bool symbol_table::exists( string lexeme )
{
    for(int i = table.size()-1 ; i >= 0 ; --i ){
        if(table[i].find(lexeme) != table[i].end())
            return true;
    }
    return false;
}

bool symbol_table::exists_last_scope( string lexeme )
{
    return table[table.size()-1].count(lexeme) != 0 ;
}

int symbol_table::operator[]( const string& lexeme)
{
    return get_symbol_pointer(lexeme);
}

void symbol_table::enter_scope()
{
    table.push_back(map<string,entry*>());
}

void symbol_table::exit_scope()
{
    table_counter -= table[table.size()-1].size();
    table.pop_back();
}


void symbol_table::print()
{
    map<int,entry> to_print;

    for(map<string,entry*> m : table){
        for(auto it = m.begin(); it != m.end(); it++){
            to_print.insert(make_pair(it->second->get_symbol_pointer(),*(it->second)));
        }
    }
    cout << "sym_ptr\tlexeme\ttype\n";
    for(auto it = to_print.begin(); it != to_print.end();it++){
        cout << it->first << "\t" << it->second.get_lexeme() << "\t" << it->second.get_type() << endl;
    }
    cout << "Symbol table: exiting scope..\n";
}


