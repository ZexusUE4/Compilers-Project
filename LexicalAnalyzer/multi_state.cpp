#include "multi_state.h"
#include <iostream>

/* initializing the static member allInstances */
vector<multi_state*> multi_state::all_instances;

multi_state::multi_state(set<state*> ownedStates){
	this->owned_states = ownedStates;
	acceptance_flag = false;
	hash_value = 0;
	size_t pi = 1;
	priority = (int)1e9;

	for (state* st : ownedStates){
		if (st->is_acceptance_state() && priority > st->priority){
			set_acceptance_state(st->name);
			priority = st->priority;
		}
		hash_value += st->id;
		pi *= st->id;
	}
	hash_value += pi;

	all_instances.push_back(this);
}

multi_state::~multi_state(){

}

void multi_state::delete_all_instances(){
	for (multi_state* st : all_instances){
		delete st;
	}

	all_instances.clear();
}

multi_state* multi_state::next_state(char transition){

	set<state*> next_states;

	for (state* st : owned_states){

		vector<state*> stNextStates = st->next_states(transition);
		next_states.insert(stNextStates.begin(), stNextStates.end());
	}

	set<state*> epsClosures;

	for (state* st : next_states){

		set<state*> epsClosure = st->get_epsilon_closure();
		epsClosures.insert(epsClosure.begin(), epsClosure.end());
	}

	next_states.insert(epsClosures.begin(), epsClosures.end());

	multi_state* newMultiState = new multi_state(next_states);

	return newMultiState;

}

vector<char> multi_state::get_valid_transtitions(bool ignoreEps){

	set<char> transitions;

	for (state* st : owned_states){

		vector<char> stValidTransitions = st->get_valid_transtitions(ignoreEps);
		transitions.insert(stValidTransitions.begin(), stValidTransitions.end());

	}

	vector<char> validTransitions(transitions.begin(), transitions.end());

	return validTransitions;
}

bool multi_state::is_valid_transition(char transition){

	bool isValidTransition = false;

	for (state* st : owned_states){
		isValidTransition |= st->is_valid_transition(transition);
	}

	return isValidTransition;
}

bool multi_state::operator == (const multi_state& rhs) const{
	return owned_states == rhs.owned_states;
}

//Don't use
bool multi_state::operator<(const multi_state& rhs) const{
	return owned_states < rhs.owned_states;
}

void multi_state::print_state(bool detailed){

	cout << "MultiState ids: {";

	for (auto it = owned_states.begin(); it != owned_states.end(); it++){

        if(it != owned_states.begin())
                cout << ", ";

		state* st = *it;
		cout << st->id;

	}
	cout << "}, ";

	if (detailed){
		cout << "MultiState names: {";

		for (auto it = owned_states.begin(); it != owned_states.end(); it++){


			state* st = *it;
			cout << st->name;
		}
		cout << "}, ";

		cout << "AcceptanceFlag: " << acceptance_flag << endl;

		cout << "Transitions: ";

		vector<char> validTransitions = get_valid_transtitions();

		for (int i = 0; i < (int)validTransitions.size(); i++){

			cout << validTransitions[i];

			if (i + 1 != (int)validTransitions.size()){
				cout << ", ";
			}
		}
	}

	cout << endl;

}
