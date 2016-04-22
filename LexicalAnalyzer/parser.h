#include "production.h"
#include <set>

class parser
{

private:

    /* a set containing all productions found in the productions file */
    set<production> productions;

public:

    /* parser constructor */
    parser(string productions_file_name);
};
