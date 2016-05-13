#include "production.h"
#include <fstream>
#include <set>
#include "semantic_functions.h"

class production_reader
{
private:

    /* prodcutions file */
    ifstream file;

    /* singleton instance of the production_reader class */
    static production_reader * instance;

    /* default constructor */
    production_reader();

    /* parses the given line to a production */
    production* parse_line_to_prodcution(string line);

    /* returns the psymbol_type of this string token */
    psymbol_type get_psymbol_type(string str);

    /* checks if this line defines a new production or to continue the previous production */
    bool is_new_production(string str);

    /* checks if this string is a semantic function */
    bool is_semantic_func(string str);

    /* splits the given string on the space */
    vector<psymbol> space_splitted(string str);

    /* Start symbol of the productions */
    psymbol start_symbol ;

public:

    /* starts reading the productions file and returns a set containing all productions */
    set<production> read(string file_name);

    /* returns the singleton instance of the production_reader class */
    static production_reader * get_instance();

    psymbol get_first_symbol();
};
