#include "NFAtoDFA.h"
#include <queue>
#include <iostream>

//Initializing singleton to null
NFAtoDFA* NFAtoDFA::instance = NULL;


NFAtoDFA::NFAtoDFA(){

}

Automata* NFAtoDFA::getDFA(Automata* NFA){

	currentNFA = NFA;

	unminimizedDFA = toDFA(currentNFA);

	minimizedDFA = minimzeDFA(unminimizedDFA);

	return minimizedDFA;
}

Automata* NFAtoDFA::toDFA(Automata* NFA){

	if (NFA->automataType != ENFA)
		return NULL;

	unordered_map<MultiState, State*> multiMap;
	queue<MultiState*> statesQueue;
	State* NFAStart = NFA->startState;

	MultiState* startSet = new MultiState(NFAStart->getEpsilonClosure());
	multiMap[*startSet] = new State(startSet->name,startSet->isAcceptanceState(),startSet->priority);

	statesQueue.push(startSet);

	while (!statesQueue.empty()){
		MultiState* topMulti = statesQueue.front();	statesQueue.pop();
		vector<char> validTransitions = topMulti->getValidTransitions(true);

		for (char c : validTransitions){
			MultiState* next = topMulti->nextState(c);

			if (multiMap.find(*next) == multiMap.end()){
				multiMap[*next] = new State(next->name, next->isAcceptanceState(),next->priority);
				multiMap[*topMulti]->addTransition(c, multiMap[*next]);

				statesQueue.push(next);
			}
			else{
				multiMap[*topMulti]->addTransition(c, multiMap[*next]);
			}
		}
	}

	Automata* convertedDFA = new Automata(multiMap[*startSet], EDFA);
	MultiState::deleteAllInstances();
	return convertedDFA;
}

Automata* NFAtoDFA::minimzeDFA(Automata* DFA){

	//TO DO Bolbol

	return DFA;
}

NFAtoDFA* NFAtoDFA::getInstance(){
	if (!instance)
		instance = new NFAtoDFA();

	return instance;
}
