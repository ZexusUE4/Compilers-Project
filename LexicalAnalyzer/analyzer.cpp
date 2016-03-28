#include "analyzer.h"

bool analyzer::isValidFile( string fname )
{
   return ( access( fname.c_str(), F_OK ) != -1 );
}

bool analyzer::file_extract( string& src , string str )
{
    int i = 0 ;
    while( str[i]!='\"' && i < str.size() )++i ;
    ++i ;

    src.clear();
    for( ; i < str.size() && str[i]!='\"' ; ++i)
        src.push_back(str[i]);

    return isValidFile(src);
}

bool analyzer::local_set( string str )
{
    istringstream is(str) ;
    string ss ;
    is >> ss ;
    if( ss == "rules"){
        is >> ss ;
        is >> ss ;
        return file_extract(rules_file,ss);
    }
    else if( ss == "compile" ){
        is >> ss ;
        is >> ss ;
        return file_extract(compile_file,ss);
    }
    else{
        return 0 ;
    }
}

analyzer::analyzer( string configuration_file )
{
    hasErr = 0 ;
    if( !isValidFile(configuration_file) ){
        hasErr = 1 ;
        return ;
    }

    ifstream conf_file(configuration_file) ;
    string str = "" ;

    getline(conf_file,str);
    if( !local_set(str) ){
        hasErr = 1 ;
        return ;
    }

    getline(conf_file,str);
    if( !local_set(str) ){
        hasErr = 1 ;
        return ;
    }

    if( !isValidFile(rules_file) || !isValidFile(compile_file) || (rules_file == compile_file) ){
        hasErr = 1 ;
        return ;
    }
}

analyzer::~analyzer()
{
    if( comp_f_stream.is_open() ){
        comp_f_stream.close();
    }
}

bool analyzer::hasError()
{
    return hasErr ;
}

void analyzer::printFilesNames()
{
    cout<< "Expression rules file : " <<rules_file<<endl ;
    cout<< "File to compile : "<<compile_file<<endl;
}

void analyzer::start()
{
    if( hasError() )return ;
    Automata* NFAmata = Parser::getInstance()->getNFA(rules_file);
	NFAtoDFA* converter = NFAtoDFA::getInstance();
	DFA = converter->getDFA(NFAmata);
    comp_f_stream.open(compile_file,ios_base::binary);
}

void analyzer::refresh()
{
    last_acceptance = NULL ;
    current_state = DFA->startState ;
}

char analyzer::read_char()
{
    return comp_f_stream.get();
}

char analyzer::peek_char()
{
    return comp_f_stream.peek();
}

void analyzer::restore_char()
{
    restore_char(1);
}

void analyzer::restore_char( int n )
{
    comp_f_stream.seekg (-n, comp_f_stream.cur);
}

void analyzer::restore_pos()
{
    restore_char(comp_f_stream.tellg()-acceptace_pos);
}

void analyzer::drop_garbage()
{
    if( peek_char() == EOF )return ;
    while(true){
        char c = read_char();
        if( !isspace(c) ){
            if( c == '/' ){
                if( peek_char() == EOF ){
                    restore_char();
                    return ;
                }
                char cc = read_char();
                if( cc == '/' ){//drop until new line is found
                    while(true){
                        cc = read_char();
                        if( cc == EOF )return ;
                        if( cc == '\n' ){
                            drop_garbage();//maybe the new line has garbage
                            return ;//return from this method
                        }
                    }
                }
                else if( cc == '*' ){//drop until */ is found
                    c = read_char();
                    while( true ){
                        if( c != EOF ){
                            cc = read_char();
                            if( cc == EOF )return ;
                            if( c == '*' && cc == '/' ){
                                drop_garbage();
                                return ;
                            }
                            c = cc ;
                        }
                        else{
                            return ;
                        }
                    }
                }
                else{//we're in good case
                    restore_char(2);
                    break ;
                }
            }
            else{
                restore_char();
                break ;/* means we start at character that is not a begin of comment */
            }
        }
        else{
            drop_garbage();
            break ;
        }
    }
}

token analyzer::getToken()
{
    refresh();
    drop_garbage();

    if( peek_char() == EOF )
        return token("END_OF_FILE","") ;

    int first_pos = comp_f_stream.tellg();
    string value = "" ;

    while( true ){
        int c = read_char();
        value.push_back(c);

        if( current_state->isValidTransition(c) ){
            current_state = current_state->nextState(c);
            if( current_state->isAcceptanceState() ){
                last_acceptance = current_state ;
                acceptace_pos = comp_f_stream.tellg();
            }
        }
        else{//dead end
            if( last_acceptance != NULL ){
                int len = comp_f_stream.tellg()-acceptace_pos ;
                for(int i = 0 ; i < len ; ++i){
                    value.pop_back();
                }
                restore_pos();
                return token(last_acceptance->name,value);
            }
            else{//error checking
                //return fix_error(token("BAD_TOKEN",value));
                return token("BAD_TOKEN",value);
            }
        }
    }
}

token analyzer::fix_error( token t )
{
    string val = t.getValue();
    string temp = val ;
    //  for(int i = 0 ; i < )
}
























