#pragma once

#include "state_base.h"
#include "state.h"
#include "multi_state.h"

/*Types of the automata:
	1- Nondeterministic.
	2- Deterministic.
	3- Minimized Deterministic. */
enum EAutomataType
{
	ENFA, EDFA, EMDFA
};

class automata
{
private:

    string num2str ( int Number );

public:

	/* Current state of this automata */
	state* current_state;

	/* Starting state of this automata */
	state* start_state;

	/* Current type of this automata */
	EAutomataType automata_type;

	/**** Functions ****/

	/* Default constructor */
	automata();

	/* Destructor that deletes all the states in this automata */
	~automata();

	/* Automata constructor */
	automata(state* startState, EAutomataType Type);

    /* Returns a vector containing all states in this Automata */
	vector<state*> get_all_states();

	vector<char> get_all_transtions();

	/* Prints all States and transitions inside this Automata*/
	void print_automata();

	/* returns the number of states in this automata */
	size_t count_states();

	/*Prints the transition table */
	void print_transition_table(string fileName = "STDOUT");

};
