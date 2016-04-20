#pragma once

#include <vector>
#include <string>
#include "state_base.h"

using namespace std;

class state : public state_base
{

private:

	/* Map containing all the transitions from this State */
	map<char, vector<state*> > transitions;

public:
    /* Dummy used for or */
	bool is_dummy;


	/* State constructor */
	state(string name = "", bool isAcceptance = false, int priority = -1,bool is_dummy = false);

	/* Destructor. It deletes all States that this State goes to*/
	~state();

	/* Checks if the input char is a valid transition for this State. */
	bool is_valid_transition(char ch) override;

	/* Returns the next State if it was valid else it returns this State. */
	state* next_state(char transition) override;

	/* Returns a vector containing all valid transitions from this State*/
	vector<char> get_valid_transtitions(bool ignoreEps = false) override;

	/* Prints the id of the state by default, also prints transitions, acceptanceFlag and name if detailed is set to true */
	void print_state(bool detailed = false) override;

	/* Returns a vector containing next States if it was valid transition else it returns a vector containing this State. */
	vector<state*> next_states(char transition);

	/* Adds a transition from this State to next_state using transitionChar. */
	void add_transiton(char transitionChar, state* next_state);

	/* Removes all transitions from this State */
	void clear_transitions();

	/* Returns the EpsilonClosure of this State */
	set<state*> get_epsilon_closure();

	/* Operator < overloading */
	bool operator<(const state &st) const;
};
