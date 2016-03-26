#include "MultiState.h"
#include <iostream>

/* initializing the static member allInstances */
vector<MultiState*> MultiState::allInstances;

MultiState::MultiState(set<State*> ownedStates){
	this->ownedStates = ownedStates;
	acceptanceFlag = false;

	priority = 1e9;
	for (State* st : ownedStates){
		if (st->isAcceptanceState() && priority > st->priority){
			setAcceptanceState(st->name);
			priority = st->priority;
		}
	}

	allInstances.push_back(this);
}

MultiState::~MultiState(){

}

void MultiState::deleteAllInstances(){
	for (MultiState* st : allInstances){
		delete st;
	}

	allInstances.clear();
}

MultiState* MultiState::nextState(char transition){

	set<State*> nextStates;

	for (State* st : ownedStates){

		vector<State*> stNextStates = st->nextStates(transition);
		nextStates.insert(stNextStates.begin(), stNextStates.end());
	}

	set<State*> epsClosures;

	for (State* st : nextStates){

		set<State*> epsClosure = st->getEpsilonClosure();
		epsClosures.insert(epsClosure.begin(), epsClosure.end());
	}

	nextStates.insert(epsClosures.begin(), epsClosures.end());

	MultiState* newMultiState = new MultiState(nextStates);

	return newMultiState;

}

vector<char> MultiState::getValidTransitions(bool ignoreEps){

	set<char> transitions;

	for (State* st : ownedStates){

		vector<char> stValidTransitions = st->getValidTransitions(ignoreEps);
		transitions.insert(stValidTransitions.begin(), stValidTransitions.end());

	}

	vector<char> validTransitions(transitions.begin(), transitions.end());

	return validTransitions;
}

bool MultiState::isValidTransition(char transition){

	bool isValidTransition = false;

	for (State* st : ownedStates){
		isValidTransition |= st->isValidTransition(transition);
	}

	return isValidTransition;
}

bool MultiState::operator == (const MultiState& rhs) const{
	return ownedStates == rhs.ownedStates;
}

bool MultiState::operator<(const MultiState& rhs) const{
	return ownedStates < rhs.ownedStates;
}

void MultiState::printState(bool detailed){

	cout << "MultiState ids: {";

	for (auto it = ownedStates.begin(); it != ownedStates.end(); it++){
		State* st = *it;
		cout << st->id;

		it++;
		if (it != ownedStates.end()){
			cout << ", ";
		}
		it--;
	}
	cout << "}, ";

	if (detailed){
		cout << "MultiState names: {";

		for (auto it = ownedStates.begin(); it != ownedStates.end(); it++){
			State* st = *it;
			cout << st->name;

			it++;
			if (it != ownedStates.end()){
				cout << ", ";
			}
			it--;
		}
		cout << "}, ";

		cout << "AcceptanceFlag: " << acceptanceFlag << endl;

		cout << "Transitions: ";

		vector<char> validTransitions = getValidTransitions();

		for (int i = 0; i < (int)validTransitions.size(); i++){

			cout << validTransitions[i];

			if (i + 1 != validTransitions.size()){
				cout << ", ";
			}
		}
	}

	cout << endl;

}
