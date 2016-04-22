#include "parser.h"
#include "production_reader.h"

parser::parser(string productions_file_name){

    productions = production_reader::get_instance()->read(productions_file_name);

    for(production p : productions){
        p.print_production();
    }
}
