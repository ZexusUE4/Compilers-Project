#pragma once

#include <map>
#include <vector>
#include <set>
#include <string>

#define Eps (char)238

using namespace std;

class StateBase
{

protected:

	/* Id Counter incremented when a State is created */
	static int idCounter;

	/* Whether this State is an acceptance State or not */
	bool acceptanceFlag;

public:

	/* Id of the State */
	int id;

	/* Name of the State */
	string name;

	/* Priority of the State*/
	int priority;


	/**** Functions ****/

	/* Default constructor */
	StateBase();

	/* Checks if the input char is a valid transition for this State. */
	virtual bool isValidTransition(char ch) = 0;		//Abstract

	/* Returns the next State if it was valid else it returns this State. */
	virtual StateBase* nextState(char transition) = 0;			//Abstract

	/* Returns a vector containing all valid transitions from this State*/
	virtual vector<char> getValidTransitions(bool ignoreEps = false) = 0;		//Abstract

	/* Prints the id of the state by default, also prints transitions, acceptanceFlag and name if detailed is set to true */
	virtual void printState(bool detailed = false) = 0;		//Abstract

	/* Marks this State as an acceptance State */
	void setAcceptanceState(string Keyword);

	/* Checks if this State is an acceptance State or not*/
	bool isAcceptanceState();
};
