#include "NFAtoDFA.h"
#include <iostream>
#include <sstream>
#include "Parser.h"
using namespace std;

int main(){

	//Example link : http://web.cecs.pdx.edu/~harry/compilers/slides/LexicalPart3.pdf
	/*State* NFAStates[11];
	for (int i = 0; i < 11; i++){
	    ostringstream stm ;
        stm << i ;
		NFAStates[i] = new State(stm.str());
	}

	NFAStates[10]->setAcceptanceState("10");

	NFAStates[0]->addTransition(Eps, NFAStates[1]);
	NFAStates[0]->addTransition(Eps, NFAStates[7]);
	NFAStates[1]->addTransition(Eps, NFAStates[4]);
	NFAStates[1]->addTransition(Eps, NFAStates[2]);
	NFAStates[2]->addTransition('a', NFAStates[3]);
	NFAStates[4]->addTransition('b', NFAStates[5]);
	NFAStates[3]->addTransition(Eps, NFAStates[6]);
	NFAStates[5]->addTransition(Eps, NFAStates[6]);
	NFAStates[6]->addTransition(Eps, NFAStates[1]);
	NFAStates[6]->addTransition(Eps, NFAStates[7]);
	NFAStates[7]->addTransition('a', NFAStates[8]);
	NFAStates[8]->addTransition('b', NFAStates[9]);
	NFAStates[9]->addTransition('b', NFAStates[10]);*/

	Automata* NFAmata = Parser::getInstance()->getNFA("test.txt");
	//NFAmata->printAutomata();
	cout << "NFA state count: " << NFAmata->countStates() << endl;

	NFAtoDFA* converter = NFAtoDFA::getInstance();
	Automata* DFA = converter->getDFA(NFAmata);

	cout << "DFA state count: " << DFA->countStates() << endl;


	//DFA->printAutomata();'
}
