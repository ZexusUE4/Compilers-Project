#ifndef ENTRY_H
#define ENTRY_H

#include<string>
#include<vector>

using namespace std ;

class entry
{
    public:
        entry();
        entry( string lexeme , string type );
        virtual ~entry();

        void setLexeme( string lex );
        void setType( string typ );
        void setInfo( string inf );
        void addExtraValue( string val );

        string getLexeme();
        string getType();
        string getInfo();
        string getExtra( int pos );
        int size_of_extra();

        bool operator <( const entry& e ) const;
    private:
        /* Only strings because they're dynamic and don't reserve extra space by default */
        string lexeme ;          /* The extracted string itself */
        string type ;            /* Lexical type */
        string info ;            /* Extra essential info value */
        vector<string> extra ;   /* Dynamic memory for any extra values */
};

#endif // ENTRY_H

