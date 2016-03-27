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

    //cout << "NFA state count: " << currentNFA->countStates() << endl;

	unminimizedDFA = toDFA(currentNFA);
    //cout << "DFA state count: " << unminimizedDFA->countStates() << endl;

	/***********Testing***********/

	//Test1
//	State* s1= new State("1", 0, -1);
//	State* s2= new State("4", 1, -1);
//	State* s3= new State("3", 0, -1);
//	State* s4= new State("4", 1, -1);
//	State* s5= new State("4", 1, -1);
//	State* s6= new State("4", 1, -1);
//	State* s7= new State("4", 1, -1);
//	State* s8= new State("4", 1, -1);
//
//	s1->addTransition('0', s2);
//	s2->addTransition('0', s3);
//	s2->addTransition('1', s4);
//	s3->addTransition('0', s5);
//	s3->addTransition('1', s3);
//	s4->addTransition('0', s6);
//	s4->addTransition('1', s3);
//	s5->addTransition('1', s7);
//	s6->addTransition('0', s8);
//	s6->addTransition('1', s4);
//	s7->addTransition('0', s6);
//	s7->addTransition('1', s3);
//	s8->addTransition('0', s5);
//	s8->addTransition('1', s4);

	//Test2
//	State* s1= new State("1", 0, -1);
//	State* s2= new State("2", 0, -1);
//	State* s3= new State("3", 0, -1);
//	State* s4= new State("4", 0, -1);
//	State* s5= new State("5", 0, -1);
//	State* s6= new State("6", 0, -1);
//	State* s7= new State("7", 1, -1);
//	State* s8= new State("7", 1, -1);
//
//	s1->addTransition('a', s4);
//	s1->addTransition('b', s2);
//	s2->addTransition('c', s3);
//	s3->addTransition('d', s8);
//	s4->addTransition('b', s5);
//	s5->addTransition('c', s6);
//	s6->addTransition('b', s7);
//
//	Automata* test= new Automata(s1, EDFA);
//	minimizedDFA = minimzeDFA(test);
	/***********End Test***********/

	//minimizedDFA = minimzeDFA(unminimizedDFA);

    //cout << "DFA minimized size= " << minimizedDFA->countStates() << endl;
//    return unminimizedDFA;
	return unminimizedDFA;
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

	vector< set<State*> > current_state_sets[2];	// Holding the sets of the states.
	State* start_state= DFA->startState;
	vector<char> possible_input;

	set<State*> accepting_states;
	set<State*> non_accepting_states;

	// Traversing and retrieving the graph states (BFS).
	queue <State*> states_queue;
	set <State*> visited;

	states_queue.push(start_state);
	visited.insert(start_state);
	non_accepting_states.insert(start_state);

	set<char> symbols; // To collect the input characters.

	while(!states_queue.empty()){
		State* current_state= states_queue.front();	states_queue.pop();
		vector<char> c= current_state->getValidTransitions();
		for(int i= 0; i< (int)c.size(); i++){
			symbols.insert(c[i]);
			State* neighbour;
			neighbour= current_state->nextState(c[i]);
			if(visited.find(neighbour) == visited.end()){	// Not visited before.
				visited.insert(neighbour);
				states_queue.push(neighbour);
				if(neighbour->isAcceptanceState()) accepting_states.insert(neighbour);
				else non_accepting_states.insert(neighbour);
			}
		}
	}
	/**************************************************************************************/

	// Moving the data from the set to the vector.
	set<char>::iterator it;
	int ind= 0;
	for(it= symbols.begin(); it!= symbols.end(); it++){
		possible_input.push_back(*it);
//		cout << (char)(*it) << endl; // Printing the input characters.
	}

	current_state_sets[0].push_back(non_accepting_states);

	/**************************************************************************************/
	// Dividing the accepting states into different accepting states according to their type.
	map<string, set<State*> > states_types;
	set<State*>::iterator accepting_state_it;
	for(accepting_state_it= accepting_states.begin(); accepting_state_it!= accepting_states.end(); ++accepting_state_it){
		states_types[(*accepting_state_it)->name].insert((*accepting_state_it));
	}

	map<string, set<State*> >::iterator states_type_it;
	for(states_type_it= states_types.begin(); states_type_it!= states_types.end(); ++states_type_it){
		current_state_sets[0].push_back(states_type_it->second);
	}

	// Debugging
//	for(int i= 0; i< (int)current_state_sets[0].size(); i++){
//		set<State*>::iterator it;
//		cout << "Set: ";
//		for(it= current_state_sets[0][i].begin(); it!=current_state_sets[0][i].end(); it++){
//			cout << (*it)->name << " ";
//		}
//		cout << endl;
//	}


	/**************************************************************************************/
	// Continuously checking for minimized sets.
	int flip= 0;

	while(true){

		// Iterating on each set.
		for(int i= 0; i< (int)current_state_sets[flip].size(); i++){
			set<State*>::iterator elem_it;
			map < vector<int>, set<State*> > brackets; // Holds the transition of each element in the current set, and collect them together.

			// Iterating on each element in the current set.
			for(elem_it= current_state_sets[flip][i].begin(); elem_it!= current_state_sets[flip][i].end(); ++elem_it){
				vector<int> current_set_neighbours;

				// Trying all possible inputs for the current state.
				for(int j= 0; j< (int)possible_input.size(); ++j){
					current_set_neighbours.push_back(getSetNumber((*elem_it)->nextState(possible_input[j]), current_state_sets[flip]));
				}
				brackets[current_set_neighbours].insert(*elem_it); 	// Check for how to insert an iterator ?
			}
			// Dividing the current set into multiple sets.
			map < vector<int>, set<State*> >::iterator brackets_it;
			for(brackets_it= brackets.begin(); brackets_it!= brackets.end(); ++brackets_it)
				current_state_sets[(flip+1)%2].push_back(brackets_it->second);
		}
		flip++;
		flip%= 2;
		if(current_state_sets[0].size() == current_state_sets[1].size()) break;
		current_state_sets[(flip+1)%2].clear();
	}

	/*********************************************************************************************/
	// Creating a state of each set.
	vector<State*> new_DFA_states((int)current_state_sets[0].size());
	for(int i= 0; i< (int)current_state_sets[0].size(); ++i){
		set<State*>::iterator first_elem= current_state_sets[0][i].begin();
		string name= (*first_elem)->name;
		bool acceptance= (*first_elem)->isAcceptanceState();
		int priority= (*first_elem)->priority;
		new_DFA_states[i]= new State(name, acceptance, priority);
	}

	// Converting into a DFA.
	Automata* minDFA= new Automata(new_DFA_states[0], EDFA);
	for(int i= 0; i< (int)current_state_sets[0].size(); ++i){
		set<State*>::iterator first_elem= current_state_sets[0][i].begin();
		for(int j= 0; j< (int)possible_input.size(); ++j){
			if((*first_elem)->isValidTransition(possible_input[j])){
				int next_state_number= getSetNumber((*first_elem)->nextState(possible_input[j]), current_state_sets[0]);
				new_DFA_states[i]->addTransition(possible_input[j], new_DFA_states[next_state_number]);
			}
		}
	}

	return minDFA;
}

int NFAtoDFA::getSetNumber(State* s, vector< set<State*> > v){
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
