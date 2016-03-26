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

	/* Prints all States and transitions inside this Automata*/
	void printAutomata();

};