#include "token.h"

token::token( string type , string value )
{
    this->type = type ;
    this->value = value ;
}

string token::getType()
{
    return type ;
}

string token::getValue()
{
    return value ;
}

