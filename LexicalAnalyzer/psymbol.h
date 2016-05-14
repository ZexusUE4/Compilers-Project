#pragma once
#include<vector>
#include<map>
#include <string>

using namespace std;

enum class psymbol_type {
    terminal,
    non_terminal,
    epsilon,
    or_operator,
    start_dummy,
    synch
};

struct psymbol_data
{
    int int_val;
    float float_val;
    int next_addr;
    string type;
    vector<int>* trueList;
    vector<int>* falseList;
};

class psymbol
{
private:
    /* string value of the production symbol */
    string value;
    string lexeme_type;

    /* type of the production symbol */
    psymbol_type type;

public:
    /* production symbol constructor */
    psymbol(string val, string lexeme_type, psymbol_type typ);
    psymbol(string val, psymbol_type typ);

    /* default constructor */
    psymbol();
    void set_val(string new_val);
    void set_lexeme_type(string new_type);
    string get_val() const ;
    string get_html_val() const;
    string get_lexeme_type() const;
    psymbol_type get_type() const ;

    /* reference to the call back function to be called when this psymbol is popped from the parser stack */
    void (*call_back) (psymbol*);
    void (*pre_call) (psymbol*);

    /* contains references to the children of this psymbol in the parser stack */
    vector<psymbol*> childs;
    psymbol* parent;

    /* used in the semantic analyzer */
    psymbol_data data;

    bool operator<(const psymbol &b) const;
    psymbol* next_sibling(int distance = 1);
    psymbol* prev_sibling(int distance = 1);
};
