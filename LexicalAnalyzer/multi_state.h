#pragma once

#include "state.h"

class multi_state : public state_base
{

private:

	/* Storing references for all instances of MultiState class to be deleted later */
	static vector<multi_state*> all_instances;

public:


    /* a Set containing all States of this MultiState*/
	set<state*> owned_states;

    /* Hash value for this multiState */
    size_t hash_value;

	/* MultiState constructor*/
	multi_state(set<state*> ownedStates);

	/* Destructor */
	~multi_state();

	/* Returns a MultiState resulted from the given transition from this MultiState, also includes the epsClosure of the resulting MultiState */
	multi_state* next_state(char transition) override;

	/* Returns all valid transitions from this MultiState*/
	vector<char> get_valid_transtitions(bool ignoreEps = false) override;

	/* Checks if the input char is a valid transition for this State. */
	bool is_valid_transition(char transition) override;

	/* Prints the id of the state by default, also prints transitions, acceptanceFlag and name if detailed is set to true */
	void print_state(bool detailed = false) override;

	/* Operator == overloading */
	bool operator==(const multi_state &rhs) const;

	/* Operator < overloading */
	bool operator<(const multi_state &rhs) const;

	/* Deletes all instances of this class */
	static void delete_all_instances();
};

namespace std{
    template<>
    struct hash<multi_state>
    {
        bool operator()(const multi_state& state) const{
            return state.hash_value;
        }
    };
}
