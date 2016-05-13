#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include "entry.h"
#include <map>
#include <string>

using namespace std ;

class symbol_table
{
    public:
        symbol_table();
        virtual ~symbol_table();

        void add( string lexeme , entry* value );
        entry* get_entry( string lexeme );
        bool exists( string lexeme );
        bool exists_last_scope( string lexeme );
        int get_symbol_pointer(string lexeme);

        void enter_scope();
        void exit_scope();

        int operator[]( const string& lexeme);

        void print();
    private:
        vector<map<string,entry*>> table ;
        int table_counter;
};

#endif // SYMBOL_TABLE_H

