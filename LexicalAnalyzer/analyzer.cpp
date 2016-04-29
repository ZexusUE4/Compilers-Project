#include "analyzer.h"


analyzer::analyzer( string rules_file_name, string compile_file_name )
{
    has_err = 0;
    this->rules_file = rules_file_name;
    this->compile_file = compile_file_name;
}

analyzer::~analyzer()
{
    if( comp_f_stream.is_open() ){
        comp_f_stream.close();
    }
}

bool analyzer::has_error()
{
    return has_err ;
}

void analyzer::print_files_names()
{
    cout<< "rules : " <<rules_file<<endl ;
    cout<< "compile : "<<compile_file<<endl;
}

void analyzer::start()
{
    if( has_error() )return ;
    automata* NFAmata = regex_reader::getInstance()->get_nfa(rules_file);
	nfa_to_dfa* converter = nfa_to_dfa::get_instance();
	DFA = converter->get_dfa(NFAmata);
    comp_f_stream.open(compile_file,ios_base::binary);
}

void analyzer::refresh()
{
    last_acceptance = NULL ;
    current_state = DFA->start_state ;
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

token analyzer::get_token()
{
    refresh();
    drop_garbage();

    if( peek_char() == EOF )
        return token("END_OF_FILE"," ") ;

    int first_pos = comp_f_stream.tellg();
    string value = "" ;

    while( true ){
        int c = peek_char();

        if( c != EOF ){
            value.push_back(c);
            read_char();
        }
        else{
            comp_f_stream.seekg(0,comp_f_stream.end);
        }

        if( current_state->is_valid_transition(c) ){
            current_state = current_state->next_state(c);
            if( current_state->is_acceptance_state() ){
                last_acceptance = current_state ;
                acceptace_pos = comp_f_stream.tellg();
            }
        }
        else{//dead end
            if( last_acceptance != NULL ){
                int len = comp_f_stream.tellg()-acceptace_pos ;
                int new_len = value.size() - len ;
                value.resize(new_len);
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
    string val = t.get_value();
    string temp = val ;
    //  for(int i = 0 ; i < )
}
























