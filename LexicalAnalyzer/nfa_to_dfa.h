#include "automata.h"

class nfa_to_dfa
{
private:

	automata* current_nfa;
	automata* unminimized_dfa;
	automata* minimized_dfa;

	/* Converts the given NFA to its unminimized DFA (Aladdin) */
	automata* to_dfa(automata* NFA);

	/* Minimize the given DFA  (Bolbol) */
	automata* minimze_dfa(automata* DFA);

	/* Singelton instance of this class */
	static nfa_to_dfa* instance;

	int get_set_number(state* s, vector< set<state*> >& v);

public:

	/**** Functions ****/

	/* Default consturctor */
	nfa_to_dfa();

	/* Singleton getter */
	static nfa_to_dfa* get_instance();

	/* Converts a given NFA to its minimized DFA */
	automata* get_dfa(automata* NFA);

};
