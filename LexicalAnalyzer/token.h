#ifndef TOKEN_H
#define TOKEN_H

#include <string>

using namespace std ;

class token
{
    public:
        token( string type , string value );

        string getType();
        string getValue();
    private:
        /* The type of the token */
        string type ;

        /* The value of the token */
        string value ;
};

#endif // TOKEN_H

