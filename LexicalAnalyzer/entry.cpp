#include "entry.h"

entry::entry()
{

}

entry::entry( string lexeme , string type )
{
    setLexeme(lexeme);
    setType(type);
}

entry::~entry()
{
    extra.clear();
}

void entry::setLexeme( string lex )
{
    lexeme = lex ;
}

void entry::setType( string typ )
{
    type = typ ;
}

void entry::setInfo( string inf )
{
    info = inf ;
}

void entry::addExtraValue( string val )
{
    extra.push_back(val);
}

string entry::getExtra( int pos )
{
    if( pos >= extra.size() )return "NOT_OCCUPIED";
    return extra[pos];
}

string entry::getInfo()
{
    return info ;
}

string entry::getLexeme()
{
    return lexeme ;
}

string entry::getType()
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












