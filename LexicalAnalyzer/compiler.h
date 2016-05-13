#ifndef COMPILER_H
#define COMPILER_H

#include "analyzer.h"
#include "symbol_table.h"
#include "parser.h"

#define INVALID_FILE 1

class compiler
{
    public:

    /* constructor taking the config file name and extracts required file names from it */
    compiler(string config_file);

    virtual ~compiler();

    /* Begins the compilation process */
    int start_compilation();

    protected:

    private:

    /* file names of the configuration files */
    string rules_file_name;
    string compile_file_name;
    string productions_file_name;

    /* error status of the compiler */
    int err_code;

    /* A reference to the lexical analyzer used by the compiler */
    analyzer* lex_analyzer;

    /* A reference to the predictive parser used by the compiler */
    parser* predictive_parser;

    /* Checks if the file name passed references a valid file */
    bool is_valid_file( string fname );

    /* Extracts the file name from this line in the configuration file */
    bool file_extract( string& file_name , string line );

    /* Sets the file names of the configuration files */
    bool local_set( string str );

};

#endif // COMPILER_H
