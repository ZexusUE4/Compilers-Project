#include "State.h"
#include <queue>
#include <iostream>

State::State(string name, bool isAcceptance , int priority ,bool isDummy){
	this->name = name;
	this->acceptanceFlag = isAcceptance;
	this->priority = priority;
	this->isDummy = isDummy;
}

State::~State(){

}

bool State::isValidTransition(char ch){

	vector<State*> nextStates = transitions[ch];

	return nextStates.size() > 0;
}

State* State::nextState(char ch){
	if (!isValidTransition(ch))
		return NULL;

	vector<State*> nextStates = transitions[ch];

	return nextStates[0];
}

vector<State*> State::nextStates(char ch){
	if (!isValidTransition(ch))
		return vector<State*>();

	return transitions[ch];
}


void State::addTransition(char transitionChar, State* nextState){
	transitions[transitionChar].push_back(nextState);
}

void State::clearTransitions(){
	transitions.clear();
}

set<State*> State::getEpsilonClosure(){

	set<State*> epsClosure;
	epsClosure.insert(this);

	queue<State*> q;
	q.push(this);

	while (!q.empty()){

		State* cur = q.front(); q.pop();
		vector<State*> nextStates = cur->nextStates(Eps);

		for (State* st : nextStates){

			if (epsClosure.find(st) == epsClosure.end()){
				q.push(st);
				epsClosure.insert(st);
			}
		}

	}

	return epsClosure;
}

vector<char> State::getValidTransitions(bool ignoreEps){

	vector<char> validTransitions;

	for (auto it = transitions.begin(); it != transitions.end(); it++){

		if (ignoreEps && it->first == Eps)
			continue;

		validTransitions.push_back(it->first);
	}

	return validTransitions;
}

void State::printState(bool detailed){

	cout << "State id: " << id;

	if (detailed){
		cout << ", ";
		cout << "name: \"" << name << "\"" <<  ", AcceptanceFlag: " << acceptanceFlag << endl;
		vector<char> validTransitions = getValidTransitions();

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

bool State::operator<(const State &st) const{
	return id < st.id;
}
