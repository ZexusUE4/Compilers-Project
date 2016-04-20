#ifndef PARSER_H_INCLUDED
#define PARSER_H_INCLUDED
#include <fstream>
#include <stack>
#include "automata.h"

enum type{
    DEF,REGX,KEYWORD,PUNC
};

class regex_reader
{
private:

	/*input file*/
    ifstream file;

	/* map for definitions to be used */
    map<string , string > def;

	/* reserved keywords */
    vector<string> keywords;

	/* all start states of all regular expressions */
    vector<state*> start_states;

	/* all regular expressions found in the file */
    vector < pair <string ,string > > regexs;

    /* vector containing delimters to be used later in matching part */
    vector< char > delim;


	/* priority of states according to their order in the file */
    int p =1;

	/* start state of NFA automata */
	state *start;

	/* singelton instance */
	static regex_reader* instance;

	/* Default Constructor */
	regex_reader();

	/* process line by line and do all needed functionalities */
    void process_line(string s);

	/* read the whole file */
    void read_file();

	/* delete all spaces before and after the string */
	void trim(string& s);

	/*get type of line ... regex , definition , keyword or punctuation*/
    int type_of_line(string s);

	/* save punctuations in a vector to use them later */
    void handle_punc(string s);

	/* save keywords in a vector to make their automata later */
    void handle_keywords(string s);

	/* get the name of the definition and its mapping */
    void handle_def(string s);

	/* make the regular expressions very simple as it will contain only single characters */
    void handle_reg(string s);

	/* used by hanlde_def and handle_reg to get the ind where = or : exists */
    int get_ind(string s , char c);

	/* get the name of the definition or regular expression */
    string get_name(string s , int ind);

	/* get the char preceded by \ if it is a reserved char */
    string get_enclosed(char c);

	/* make an automata for - symbol */
    void decode_or(stack<pair<state* , state*> > &st,int from,int to);

    string enclose_or(int from, int to);

	/* it is used by handle_reg and handle_def to get a simple expression to be used later */
    string decode(string s,int ind);

	/* check if it is a reserved symbol */
    bool is_reserved_symbol(char c);

	/* make an automata for keywords given */
    void create_keywords_states();

    void create_delim_states();

	/* evaluate the expressions to get the automata */
    void evaluate_regex();
	/* create two states separated by a char */
    void create_state(stack<pair<state* , state*> > &st , string name,char c);

	/* concatenate two states and separate them by an epsilon char.. the start of the resulting state will be the start of the first and the end will be from the second*/
    void concatenate(stack<pair<state* , state*> > &st);

	/*get the automata of one or more expression */
    void one_or_more(stack<pair<state* , state*> > &st);

	/*get the automata of zero or more expression */
    void zero_or_more(stack<pair<state* , state*> > &st);

	/* get the automata of or operation */
    void or_op(stack<pair<state* , state*> > &st);

	/* decide which funciton will be called depending on the operation used*/
    void solve(stack<pair<state* , state*> > &st, char c,char from , char to);

	/* make one start for all automatas */
    void make_one_start();



public:

	/* Returns the NFA of the given input regex file */
	automata* get_nfa(string filename);

	/* Returns the singelton instance of this class */
	static regex_reader* getInstance();
};


#endif // PARSER_H_INCLUDED
