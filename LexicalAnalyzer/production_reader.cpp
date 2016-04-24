#include "production_reader.h"
#include <iostream>
#include <sstream>

production_reader * production_reader::instance = 0;

production_reader * production_reader::get_instance(){
    if(instance == NULL){
        instance = new production_reader();
    }
    return instance;
}

production_reader::production_reader(){

}

set<production> production_reader::read(string file_name){
    file.open(file_name, ifstream::in);

    set<production> ret;
    string line;
    production* prev = NULL;

    while(getline(file,line)){

        if(is_new_production(line)){

            if(prev)
                ret.insert(*prev);

            prev = parse_line_to_prodcution(line);
        }
        else{
            vector<psymbol> splitted = space_splitted(line);
            prev->rhs.insert(prev->rhs.end(),splitted.begin(),splitted.end());
        }
    }

    ret.insert(*prev);

    return ret;
}

production* production_reader::parse_line_to_prodcution(string line){

    vector<psymbol> splitted = space_splitted(line);

    return new production(splitted[1],vector<psymbol>(splitted.begin() + 3,splitted.end()));
}

vector<psymbol> production_reader::space_splitted(string str){

    stringstream ss(str);

    vector<psymbol> tokens;

    while(ss){
        string t;
        ss >> t;

        if(t.size()){
            tokens.push_back(psymbol(t,get_psymbol_type(t)));
        }
    }

    return tokens;
}

psymbol_type production_reader::get_psymbol_type(string str){

    if(str[0] == str[str.size()-1] && str[0] == '\'')
        return psymbol_type::terminal;

    else if(str == "\\L")
        return psymbol_type::epsilon;

    else if(str == "|")
        return psymbol_type::or_operator;
    else
        return psymbol_type::non_terminal;
}

bool production_reader::is_new_production(string line){
    int i = 0;
    while(line[i] == ' ') i++;

    return line[i] == '#';
}
