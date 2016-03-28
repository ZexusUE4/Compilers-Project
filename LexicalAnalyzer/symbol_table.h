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

        void add( string lexeme , entry value );
        entry get( string lexeme );
        bool exists( string lexeme );

        entry& operator[]( const string& lexeme);
    private:
        map<string,entry> table ;
};

#endif // SYMBOL_TABLE_H

