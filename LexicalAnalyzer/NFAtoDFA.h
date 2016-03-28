#include "Automata.h"

class NFAtoDFA
{
private:

	Automata* currentNFA;
	Automata* unminimizedDFA;
	Automata* minimizedDFA;

	/* Converts the given NFA to its unminimized DFA (Aladdin) */
	Automata* toDFA(Automata* NFA);

	/* Minimize the given DFA  (Bolbol) */
	Automata* minimzeDFA(Automata* DFA);

	/* Singelton instance of this class */
	static NFAtoDFA* instance;

	int getSetNumber(State* s, vector< set<State*> >& v);

public:

	/**** Functions ****/

	/* Default consturctor */
	NFAtoDFA();

	/* Singleton getter */
	static NFAtoDFA* getInstance();

	/* Converts a given NFA to its minimized DFA */
	Automata* getDFA(Automata* NFA);

};
