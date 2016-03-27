#ifndef ANALYZER_H
#define ANALYZER_H

#include "token.h"
#include <fstream>
#include <sstream>
#include <unistd.h>
#include "Parser.h"
#include "NFAtoDFA.h"
#include <iostream>

#define dbg(n) cout<<"Debug "<<n<<endl
#define debg(s,n) cout<<s<<" : "<<n<<endl

class analyzer
{
    public:
        analyzer( string configuration_file );
        virtual ~analyzer();

        token getToken();                              /* Get next token , for compiler parser */
        bool hasError();                               /* IF the file reading has errors */
        void printFilesNames();                        /* Print compile and lexical rules file names */
        void start();                                  /* Only if has error is false , make DFA */

    private:
        bool local_set( string str );                  /* Setter to rules and compile files */
        bool file_extract( string& src , string str ); /* Get the file name from double quotes "fname" */
        bool isValidFile( string fname );              /* Check if the string represents exists file */
        token fix_error( token err_t );                /* Try to fix the token to match the DFA */
        void refresh();                                /* Refresh states to look for new token */
        char read_char();                              /* Get one character from the stream */
        void restore_char();                           /* Move one position back into stream */
        void restore_char( int n );                    /* Move n positions back into stream */
        void restore_pos();                            /* Seek to position of last acceptance in stream */
        void drop_garbage();                           /* Ignore white spaces and comments */

        string rules_file ;                            /* To send it to the Lex file parser */
        string compile_file ;                          /* The target file to be analyzed */
        bool hasErr ;
        ifstream comp_f_stream ;                       /* A stream to tokenize */
        Automata* DFA ;
        State* current_state ;
        State* last_acceptance ;
        int acceptace_pos ;                            /* to return to correct position in stream */
};

#endif // ANALYZER_H
