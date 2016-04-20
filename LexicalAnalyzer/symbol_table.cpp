#include "symbol_table.h"

symbol_table::symbol_table()
{

}

symbol_table::~symbol_table()
{
    table.clear();
}

void symbol_table::add( string lexeme , entry value )
{
    table[lexeme] = value ;
}

entry symbol_table::get( string lexeme )
{
    return table[lexeme] ;
}

bool symbol_table::exists( string lexeme )
{
    return table.count(lexeme) != 0 ;
}

entry& symbol_table::operator[]( const string& lexeme)
{
    return table[lexeme] ;
}








