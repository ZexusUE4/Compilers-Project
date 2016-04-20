#ifndef TOKEN_H
#define TOKEN_H

#include <string>

using namespace std ;

class token
{
    public:
        token( string type , string value );

        string get_type();
        string get_value();
    private:
        /* The type of the token */
        string type ;

        /* The value of the token */
        string value ;
};

#endif // TOKEN_H

