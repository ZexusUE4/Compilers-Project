#pragma once

#include "StateBase.h"
#include "State.h"
#include "MultiState.h"

/*Types of the automata:
	1- Nondeterministic.
	2- Deterministic.
	3- Minimized Deterministic. */
enum EAutomataType
{
	ENFA, EDFA, EMDFA
};

class Automata
{
private:

    string num2str ( int Number );

public:

	/* Current state of this automata */
	State* currentState;

	/* Starting state of this automata */
	State* startState;

	/* Current type of this automata */
	EAutomataType automataType;

	/**** Functions ****/

	/* Default constructor */
	Automata();

	/* Destructor that deletes all the states in this automata */
	~Automata();

	/* Automata constructor */
	Automata(State* startState, EAutomataType Type);

    /* Returns a vector containing all states in this Automata */
	vector<State*> getAllStates();

	vector<char> getAllTransitions();

	/* Prints all States and transitions inside this Automata*/
	void printAutomata();

	/* returns the number of states in this automata */
	size_t countStates();

	/*Prints the transition table */
	void printTransitionTable(string fileName = "STDOUT");

};
