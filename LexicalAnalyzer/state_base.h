#pragma once

#include <map>
#include <map>
#include <set>
#include <vector>
#include <set>
#include <string>

#define Eps (char)238

using namespace std;

class state_base
{

protected:

	/* Id Counter incremented when a State is created */
	static int id_counter;

	/* Whether this State is an acceptance State or not */
	bool acceptance_flag;

public:

	/* Id of the State */
	int id;

	/* Name of the State */
	string name;

	/* Priority of the State*/
	int priority;


	/**** Functions ****/

	/* Default constructor */
	state_base();

	/* Default destructor, virtual since it is a base class */
	virtual ~state_base();

	/* Checks if the input char is a valid transition for this State. */
	virtual bool is_valid_transition(char ch) = 0;		//Abstract

	/* Returns the next State if it was valid else it returns this State. */
	virtual state_base* next_state(char transition) = 0;			//Abstract

	/* Returns a vector containing all valid transitions from this State*/
	virtual vector<char> get_valid_transtitions(bool ignoreEps = false) = 0;		//Abstract

	/* Prints the id of the state by default, also prints transitions, acceptanceFlag and name if detailed is set to true */
	virtual void print_state(bool detailed = false) = 0;		//Abstract

	/* Marks this State as an acceptance State */
	void set_acceptance_state(string Keyword);

	/* Checks if this State is an acceptance State or not*/
	bool is_acceptance_state();
};
