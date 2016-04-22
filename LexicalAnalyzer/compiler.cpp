#include "compiler.h"

compiler::compiler(string configuration_file)
{
    err_code = 0;
    if( !is_valid_file(configuration_file) ){
        err_code = INVALID_FILE ;
        return ;
    }

    ifstream conf_file(configuration_file) ;
    string str = "" ;

    for(int i=0; i < 3;i++){
        getline(conf_file,str);
        if( !local_set(str) ){
            err_code = INVALID_FILE ;
            return ;
        }
    }

    if( !is_valid_file(rules_file_name) || !is_valid_file(compile_file_name) || !is_valid_file(productions_file_name)){
        err_code = INVALID_FILE ;
        return ;
    }

    lex_analyzer = new analyzer(rules_file_name,compile_file_name);
    predictive_parser = new parser(productions_file_name);

}

compiler::~compiler(){

}

bool compiler::is_valid_file( string fname )
{
   return ( access( fname.c_str(), F_OK ) != -1 );
}

bool compiler::file_extract( string& file_name , string str )
{
    int i = 0 ;
    while( str[i]!='\"' && i < str.size() )++i ;
    ++i ;

    file_name.clear();
    for( ; i < str.size() && str[i]!='\"' ; ++i)
        file_name.push_back(str[i]);

    return is_valid_file(file_name);
}

bool compiler::local_set( string str )
{
    istringstream is(str) ;
    string ss ;
    is >> ss ;
    if( ss == "rules"){
        is >> ss ;
        is >> ss ;
        return file_extract(rules_file_name,ss);
    }
    else if( ss == "compile" ){
        is >> ss ;
        is >> ss ;
        return file_extract(compile_file_name,ss);
    }
    else if(ss == "productions"){
        is >> ss;
        is >> ss;
        return file_extract(productions_file_name,ss);
    }
    else{
        return 0 ;
    }
}

int compiler::start_compilation(){

    lex_analyzer->start();
    sym_table  = new symbol_table();

    ofstream out("out.txt");

    if( err_code != 0 ){
        return err_code;
    }

    while(true){
        token t = lex_analyzer->get_token();
        if( !sym_table->exists(t.get_value()) ){
            sym_table->add(t.get_value(),entry(t.get_value(),t.get_type()));
        }

        out<<t.get_type()<<endl;
        if(t.get_type() == "END_OF_FILE")break ;
    }

    out.close();
    return 0;
}
