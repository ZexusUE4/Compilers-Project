#include "entry.h"

entry::entry()
{

}

entry::entry( string lexeme , string type )
{
    set_lexeme(lexeme);
    set_type(type);
}

entry::~entry()
{
    extra.clear();
}

void entry::set_lexeme( string lex )
{
    lexeme = lex ;
}

void entry::set_type( string typ )
{
    type = typ ;
}

void entry::set_info( string inf )
{
    info = inf ;
}

void entry::add_extra_value( string val )
{
    extra.push_back(val);
}

string entry::get_extra( int pos )
{
    if( pos >= extra.size() )return "NOT_OCCUPIED";
    return extra[pos];
}

string entry::get_info()
{
    return info ;
}

string entry::get_lexeme()
{
    return lexeme ;
}

string entry::get_type()
{
    return type ;
}

int entry::size_of_extra()
{
    return extra.size();
}

bool entry::operator <( const entry& e ) const
{
    if( lexeme == e.lexeme ){
        if( type == e.type ){
            if( info == e.info ){
                return extra < e.extra ;
            }
            else{
                return info < e.info ;
            }
        }
        else{
            return type < e.type ;
        }
    }
    else{
        return lexeme < e.lexeme ;
    }
}












