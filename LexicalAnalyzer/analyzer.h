#ifndef ANALYZER_H
#define ANALYZER_H

#include "token.h"
#include <fstream>
#include <sstream>
#include <unistd.h>
#include "regex_reader.h"
#include "nfa_to_dfa.h"
#include <iostream>

#define dbg(n) cout<<"Debug "<<n<<endl
#define debg(s,n) cout<<s<<" : "<<n<<endl

class analyzer
{
    public:
        analyzer( string rules_file_name, string compile_file_name );
        virtual ~analyzer();

        token get_token();                              /* Get next token , for compiler parser */
        bool has_error();                               /* IF the file reading has errors */
        void print_files_names();                        /* Print compile and lexical rules file names */
        void start();                                  /* Only if has error is false , make DFA */

    private:
        bool local_set( string str );                  /* Setter to rules and compile files */
        bool file_extract( string& src , string str ); /* Get the file name from double quotes "fname" */
        bool is_valid_file( string fname );              /* Check if the string represents exists file */
        token fix_error( token err_t );                /* Try to fix the token to match the DFA */
        void refresh();                                /* Refresh states to look for new token */
        char read_char();                              /* Get one character from the stream */
        char peek_char();                              /* Peek one character from the stream */
        void restore_char();                           /* Move one position back into stream */
        void restore_char( int n );                    /* Move n positions back into stream */
        void restore_pos();                            /* Seek to position of last acceptance in stream */
        void drop_garbage();                           /* Ignore white spaces and comments */

        string rules_file ;                            /* To send it to the Lex file parser */
        string compile_file ;                          /* The target file to be analyzed */
        bool has_err ;
        ifstream comp_f_stream ;                       /* A stream to tokenize */
        automata* DFA ;
        state* current_state ;
        state* last_acceptance ;
        int acceptace_pos ;                            /* to return to correct position in stream */
};

#endif // ANALYZER_H
