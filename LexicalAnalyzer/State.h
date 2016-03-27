#pragma once

#include <vector>
#include <string>
#include "StateBase.h"

using namespace std;

class State : public StateBase
{

private:

	/* Map containing all the transitions from this State */
	map<char, vector<State*> > transitions;

public:
    /* Dummy used for or */
	bool isDummy;


	/* State constructor */
	State(string name = "", bool isAcceptance = false, int priority = -1,bool isDummy = false);

	/* Destructor. It deletes all States that this State goes to*/
	~State();

	/* Checks if the input char is a valid transition for this State. */
	bool isValidTransition(char ch) override;

	/* Returns the next State if it was valid else it returns this State. */
	State* nextState(char transition) override;

	/* Returns a vector containing all valid transitions from this State*/
	vector<char> getValidTransitions(bool ignoreEps = false) override;

	/* Prints the id of the state by default, also prints transitions, acceptanceFlag and name if detailed is set to true */
	void printState(bool detailed = false) override;

	/* Returns a vector containing next States if it was valid transition else it returns a vector containing this State. */
	vector<State*> nextStates(char transition);

	/* Adds a transition from this State to nextState using transitionChar. */
	void addTransition(char transitionChar, State* nextState);

	/* Removes all transitions from this State */
	void clearTransitions();

	/* Returns the EpsilonClosure of this State */
	set<State*> getEpsilonClosure();

	/* Operator < overloading */
	bool operator<(const State &st) const;
};
