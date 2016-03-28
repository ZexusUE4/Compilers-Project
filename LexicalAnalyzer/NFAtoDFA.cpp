#include "NFAtoDFA.h"
#include <queue>
#include <iostream>
#include <vector>
#include <map>
#include <set>

//Initializing singleton to null
NFAtoDFA* NFAtoDFA::instance = NULL;


NFAtoDFA::NFAtoDFA(){

}

Automata* NFAtoDFA::getDFA(Automata* NFA){

	currentNFA = NFA;

	unminimizedDFA = toDFA(currentNFA);

	//cout << unminimizedDFA->countStates() << endl;

	minimizedDFA = minimzeDFA(unminimizedDFA);

    //cout << minimizedDFA->countStates() << endl;

	return minimizedDFA;
}

Automata* NFAtoDFA::toDFA(Automata* NFA){

	if (NFA->automataType != ENFA)
		return NULL;

	map<MultiState, State*> multiMap;
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

    vector<char> possible_input = DFA->getAllTransitions();
	set<State*> accepting_states;
	set<State*> non_accepting_states;
    vector <State*> visited = DFA->getAllStates();

    vector< set<State*> > current_state_sets[2];	// Holding the sets of the states.

	for(State * st:visited){
        if(st->isAcceptanceState()){
            accepting_states.insert(st);
        }
        else{
            non_accepting_states.insert(st);
        }
	}

	current_state_sets[0].push_back(non_accepting_states);

	// Dividing the accepting states into different accepting states according to their type.
	map<string, set<State*> > states_types;

	for(State* acc_state : accepting_states){
		states_types[acc_state->name].insert(acc_state);
	}

	for(auto states_type_it = states_types.begin(); states_type_it != states_types.end(); ++states_type_it){
		current_state_sets[0].push_back(states_type_it->second);
	}

	// Continuously checking for minimized sets.
	int flip= 0;

	while(true){

		// Iterating on each set.
		for(int i= 0; i< (int)current_state_sets[flip].size(); i++){
			map < vector<int>, set<State*> > brackets; // Holds the transition of each element in the current set, and collect them together.

			// Iterating on each element in the current set.
			for(State* st : current_state_sets[flip][i]){
				vector<int> current_set_neighbours;
				// Trying all possible inputs for the current state.
				for(char c : possible_input){
                    current_set_neighbours.push_back(getSetNumber(st->nextState(c), current_state_sets[flip]));
				}
				brackets[current_set_neighbours].insert(st);
			}

			// Dividing the current set into multiple sets.
			for(auto brackets_it= brackets.begin(); brackets_it!= brackets.end(); ++brackets_it)
				current_state_sets[flip^1].push_back(brackets_it->second);
		}
		flip^=1;

		if(current_state_sets[0].size() == current_state_sets[1].size())
            break;

        current_state_sets[flip^1].clear();
	}

	// Creating a state of each set.
	vector<State*> new_DFA_states((int)current_state_sets[0].size());

	for(int i= 0; i < (int)current_state_sets[0].size(); ++i){

		auto first_elem = current_state_sets[0][i].begin();
		string name= (*first_elem)->name;

		bool acceptance= (*first_elem)->isAcceptanceState();
		int priority= (*first_elem)->priority;

		new_DFA_states[i]= new State(name, acceptance, priority);
	}

    int new_starting_state= getSetNumber(DFA->startState, current_state_sets[0]);

	// Converting into a DFA.
	Automata* minDFA= new Automata(new_DFA_states[new_starting_state], EMDFA);

	for(int i= 0; i< (int)current_state_sets[0].size(); ++i){

		auto first_elem= current_state_sets[0][i].begin();

		for(char c : possible_input){
			if((*first_elem)->isValidTransition(c)){
				int next_state_number= getSetNumber((*first_elem)->nextState(c), current_state_sets[0]);
				new_DFA_states[i]->addTransition(c, new_DFA_states[next_state_number]);
			}
		}
	}

	return minDFA;
}

int NFAtoDFA::getSetNumber(State* s, vector< set<State*> >& v){
	if(s == NULL) return -1;
	for(int i= 0; i< v.size(); ++i){
		if(v[i].find(s) != v[i].end()) return i;
	}
}

NFAtoDFA* NFAtoDFA::getInstance(){
	if (!instance)
		instance = new NFAtoDFA();

	return instance;
}
