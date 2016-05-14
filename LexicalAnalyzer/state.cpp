#include "state.h"
#include <queue>
#include <iostream>

state::state(string name, bool isAcceptance , int priority ,bool is_dummy){
	this->name = name;
	this->acceptance_flag = isAcceptance;
	this->priority = priority;
	this->is_dummy = is_dummy;
}

state::~state(){

}

bool state::is_valid_transition(char ch){

	vector<state*> next_states = transitions[ch];

	return next_states.size() > 0;
}

state* state::next_state(char ch){
	if (!is_valid_transition(ch))
		return NULL;

	vector<state*> next_states = transitions[ch];

	return next_states[0];
}

vector<state*> state::next_states(char ch){
	if (!is_valid_transition(ch))
		return vector<state*>();

	return transitions[ch];
}


void state::add_transiton(char transitionChar, state* next_state){
	transitions[transitionChar].push_back(next_state);
}

void state::clear_transitions(){
	transitions.clear();
}

set<state*> state::get_epsilon_closure(){

	set<state*> epsClosure;
	epsClosure.insert(this);

	queue<state*> q;
	q.push(this);

	while (!q.empty()){

		state* cur = q.front(); q.pop();
		vector<state*> next_states = cur->next_states(Eps);

		for (state* st : next_states){

			if (epsClosure.find(st) == epsClosure.end()){
				q.push(st);
				epsClosure.insert(st);
			}
		}

	}

	return epsClosure;
}

vector<char> state::get_valid_transtitions(bool ignoreEps){

	vector<char> validTransitions;

	for (auto it = transitions.begin(); it != transitions.end(); it++){

		if ((ignoreEps && it->first == Eps) || (it->second).size() == 0)
			continue;

		validTransitions.push_back(it->first);
	}

	return validTransitions;
}

void state::print_state(bool detailed){

	cout << "State id: " << id;

	if (detailed){
		cout << ", ";
		cout << "name: \"" << name << "\"" <<  ", AcceptanceFlag: " << acceptance_flag << endl;
		vector<char> validTransitions = get_valid_transtitions();

		cout << "Transitions: ";

		for (int i = 0; i < (int)validTransitions.size(); i++){

			cout << validTransitions[i];

			if (i + 1 != (int)validTransitions.size()){
				cout << ", ";
			}
		}

	}

	cout << endl;
}

bool state::operator<(const state &st) const{
	return id < st.id;
}
