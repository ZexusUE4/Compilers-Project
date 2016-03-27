#include "Automata.h"
#include <queue>
#include <iostream>

Automata::Automata(){

}

Automata::Automata(State* startState, EAutomataType Type){

	this->startState = startState;
	automataType = Type;

}

Automata::~Automata(){

	queue<State*> q;
	set<State*> visited;
	q.push(startState);
	visited.insert(startState);

	while (!q.empty()){
		State* top = q.front(); q.pop();
		vector<char> validTransitions = top->getValidTransitions();

		for (char c : validTransitions){
			State* neigh = top->nextState(c);

			if (visited.find(neigh) == visited.end()){
				visited.insert(neigh);
				q.push(neigh);
			}

		}
		delete top;
	}

}

void Automata::printAutomata(){


	set<State*> visited;
	queue<State*> q;

	q.push(startState);
	visited.insert(startState);

	while (!q.empty()){
		State* top = q.front();
		q.pop();

		top->printState(true);

		vector<char> validTransitions = top->getValidTransitions();

		for (char c : validTransitions){
			vector<State*> nextStates = top->nextStates(c);

			cout << c << "  -->  ";

			for (State* st : nextStates){
				st->printState();

				if (visited.find(st) == visited.end()){
					visited.insert(st);
					q.push(st);
				}
			}
			cout << endl;
		}
	}
}

size_t Automata::countStates(){

    set<State*> visited;
    queue<State*> q;
    q.push(startState);
    visited.insert(startState);

    while(!q.empty()){
        State* top = q.front(); q.pop();
        vector<char> validTransitions = top->getValidTransitions();

        for(char c : validTransitions){
            vector<State*> nextStates = top->nextStates(c);

            for(State* st: nextStates){
                if(visited.find(st) == visited.end()){
                    visited.insert(st);
                    q.push(st);
                }
            }
        }
    }

    return visited.size();
}
