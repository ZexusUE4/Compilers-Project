#ifndef PARSER_H_INCLUDED
#define PARSER_H_INCLUDED
#include <fstream>
#include <stack>
#include "Automata.h"

enum type{
    DEF,REGX,KEYWORD,PUNC
};

class Parser
{
private:

	/*input file*/
    ifstream file;

	/* map for definitions to be used */
    map<string , string > def;

	/* reserved keywords */
    vector<string> keywords;

	/* all start states of all regular expressions */
    vector<State*> start_states;

	/* all regular expressions found in the file */
    vector < pair <string ,string > > regexs;

	/* priority of states according to their order in the file */
    int p =1;

	/* start state of NFA automata */
	State *start;

	/* singelton instance */
	static Parser* instance;

	/* Default Constructor */
	Parser();

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

	/* convert -  to all chars from the first char to the last separated by ors */
    string decode_or(int from,int to);

	/* it is used by handle_reg and handle_def to get a simple expression to be used later */
    string decode(string s,int ind);

	/* check if it is a reserved symbol */
    bool is_reserved_symbol(char c);

	/* make an automata for keywords given */
    void create_keywords_states();

	/* evaluate the expressions to get the automata */
    void evaluate_regex();
	/* create two states separated by a char */
    void create_state(stack<pair<State* , State*> > &st , string name,char c);

	/* concatenate two states and separate them by an epsilon char.. the start of the resulting state will be the start of the first and the end will be from the second*/
    void concatenate(stack<pair<State* , State*> > &st);

	/*get the automata of one or more expression */
    void one_or_more(stack<pair<State* , State*> > &st);

	/*get the automata of zero or more expression */
    void zero_or_more(stack<pair<State* , State*> > &st);

	/* get the automata of or operation */
    void or_op(stack<pair<State* , State*> > &st);

	/* decide which funciton will be called depending on the operation used*/
    void solve(stack<pair<State* , State*> > &st, char c);

	/* make one start for all automatas */
    void make_one_start();
public:

	/* vector containing delimters to be used later in matching part */
    vector< char > delim;

	/* Returns the NFA of the given input regex file */
	Automata* getNFA(string filename);

	/* Returns the singelton instance of this class */
	static Parser* getInstance();
};


#endif // PARSER_H_INCLUDED
