#include "nfa_to_dfa.h"
#include <queue>
#include <iostream>
#include <vector>
#include <map>
#include <set>

//Initializing singleton to null
nfa_to_dfa* nfa_to_dfa::instance = NULL;


nfa_to_dfa::nfa_to_dfa(){

}

automata* nfa_to_dfa::get_dfa(automata* NFA){

	current_nfa = NFA;

	unminimized_dfa = to_dfa(current_nfa);

	//cout << unminimizedDFA->countStates() << endl;

	minimized_dfa = minimze_dfa(unminimized_dfa);
	minimized_dfa->print_transition_table("table.txt");

    //cout << minimizedDFA->countStates() << endl;

	return minimized_dfa;
}

automata* nfa_to_dfa::to_dfa(automata* NFA){

	if (NFA->automata_type != ENFA)
		return NULL;

	map<multi_state, state*> multiMap;
	queue<multi_state*> statesQueue;
	state* NFAStart = NFA->start_state;

	multi_state* startSet = new multi_state(NFAStart->get_epsilon_closure());
	multiMap[*startSet] = new state(startSet->name,startSet->is_acceptance_state(),startSet->priority);

	statesQueue.push(startSet);

	while (!statesQueue.empty()){
		multi_state* topMulti = statesQueue.front();	statesQueue.pop();
		vector<char> validTransitions = topMulti->get_valid_transtitions(true);

		for (char c : validTransitions){
			multi_state* next = topMulti->next_state(c);

			if (multiMap.find(*next) == multiMap.end()){
				multiMap[*next] = new state(next->name, next->is_acceptance_state(),next->priority);
				multiMap[*topMulti]->add_transiton(c, multiMap[*next]);

				statesQueue.push(next);
			}
			else{
				multiMap[*topMulti]->add_transiton(c, multiMap[*next]);
			}
		}
	}


	automata* convertedDFA = new automata(multiMap[*startSet], EDFA);

	multi_state::delete_all_instances();
	return convertedDFA;
}

automata* nfa_to_dfa::minimze_dfa(automata* DFA){

    vector<char> possible_input = DFA->get_all_transtions();
	set<state*> accepting_states;
	set<state*> non_accepting_states;
    vector <state*> visited = DFA->get_all_states();

    vector< set<state*> > current_state_sets[2];	// Holding the sets of the states.

	for(state * st:visited){
        if(st->is_acceptance_state()){
            accepting_states.insert(st);
        }
        else{
            non_accepting_states.insert(st);
        }
	}

	current_state_sets[0].push_back(non_accepting_states);

	// Dividing the accepting states into different accepting states according to their type.
	map<string, set<state*> > states_types;

	for(state* acc_state : accepting_states){
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
			map < vector<int>, set<state*> > brackets; // Holds the transition of each element in the current set, and collect them together.

			// Iterating on each element in the current set.
			for(state* st : current_state_sets[flip][i]){
				vector<int> current_set_neighbours;
				// Trying all possible inputs for the current state.
				for(char c : possible_input){
                    current_set_neighbours.push_back(get_set_number(st->next_state(c), current_state_sets[flip]));
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
	vector<state*> new_DFA_states((int)current_state_sets[0].size());

	for(int i= 0; i < (int)current_state_sets[0].size(); ++i){

		auto first_elem = current_state_sets[0][i].begin();
		string name= (*first_elem)->name;

		bool acceptance= (*first_elem)->is_acceptance_state();
		int priority= (*first_elem)->priority;

		new_DFA_states[i]= new state(name, acceptance, priority);
	}

    int new_starting_state= get_set_number(DFA->start_state, current_state_sets[0]);

	// Converting into a DFA.
	automata* minDFA= new automata(new_DFA_states[new_starting_state], EMDFA);

	for(int i= 0; i< (int)current_state_sets[0].size(); ++i){

		auto first_elem= current_state_sets[0][i].begin();

		for(char c : possible_input){
			if((*first_elem)->is_valid_transition(c)){
				int next_state_number= get_set_number((*first_elem)->next_state(c), current_state_sets[0]);
				new_DFA_states[i]->add_transiton(c, new_DFA_states[next_state_number]);
			}
		}
	}

	return minDFA;
}

int nfa_to_dfa::get_set_number(state* s, vector< set<state*> >& v){
	if(s == NULL) return -1;
	for(int i= 0; i< v.size(); ++i){
		if(v[i].find(s) != v[i].end()) 
			return i;
	}
}

nfa_to_dfa* nfa_to_dfa::get_instance(){
	if (!instance)
		instance = new nfa_to_dfa();

	return instance;
}
