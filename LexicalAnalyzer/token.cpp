#include "token.h"

token::token( string type , string value )
{
    this->type = type ;
    this->value = value ;
}

string token::get_type()
{
    return type ;
}

string token::get_value()
{
    return value ;
}

