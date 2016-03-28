#pragma once

#include "State.h"

class MultiState : public StateBase
{

private:

	/* Storing references for all instances of MultiState class to be deleted later */
	static vector<MultiState*> allInstances;

public:


    /* a Set containing all States of this MultiState*/
	set<State*> ownedStates;

    /* Hash value for this multiState */
    size_t hashValue;

	/* MultiState constructor*/
	MultiState(set<State*> ownedStates);

	/* Destructor */
	~MultiState();

	/* Returns a MultiState resulted from the given transition from this MultiState, also includes the epsClosure of the resulting MultiState */
	MultiState* nextState(char transition) override;

	/* Returns all valid transitions from this MultiState*/
	vector<char> getValidTransitions(bool ignoreEps = false) override;

	/* Checks if the input char is a valid transition for this State. */
	bool isValidTransition(char transition) override;

	/* Prints the id of the state by default, also prints transitions, acceptanceFlag and name if detailed is set to true */
	void printState(bool detailed = false) override;

	/* Operator == overloading */
	bool operator==(const MultiState &rhs) const;

	/* Operator < overloading */
	bool operator<(const MultiState &rhs) const;

	/* Deletes all instances of this class */
	static void deleteAllInstances();
};

namespace std{
    template<>
    struct hash<MultiState>
    {
        bool operator()(const MultiState& state) const{
            return state.hashValue;
        }
    };
}
