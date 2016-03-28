#include "Automata.h"
#include <queue>
#include <iostream>
#include <fstream>
#include <sstream>

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

vector<State*> Automata::getAllStates(){

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

    return vector<State*>(visited.begin(),visited.end());

}

vector<char> Automata::getAllTransitions(){

    set<char> allTransitions;

    vector<State*> allStates = getAllStates();

    for(State * st: allStates){
        vector<char> validTransitions = st->getValidTransitions();
        allTransitions.insert(validTransitions.begin(),validTransitions.end());
    }

    return vector<char>(allTransitions.begin(),allTransitions.end());
}

void Automata::printTransitionTable(string fileName){
    streambuf *backup;
    backup = std::cout.rdbuf();     // back up cout's streambuf
    std::ofstream out;

    if(fileName != "STDOUT"){
        out.open(fileName);
        std::streambuf *coutbuf = std::cout.rdbuf(); //save old buf
        std::cout.rdbuf(out.rdbuf()); //redirect std::cout to out.txt
    }


    vector<State*> allStates = getAllStates();
    vector<char> allTransitions = getAllTransitions();

    string stId = "--------------";
    string colSep = "--------";
    string accNameSep = "------------";

    cout << stId;
    cout << accNameSep;
        //      State id
    for(char c: allTransitions){
        cout << colSep;
    }
    cout << endl;
    cout << "   State id   ";
    cout << "| Acceptance ";

    for(char c: allTransitions){
        cout << "|   " << c << "   ";
    }
    cout << endl;

    for(State* st: allStates){
        cout << stId;
        cout << accNameSep;
        for(int i = 0;i < allTransitions.size();i++){
            cout << colSep;
        }
        cout << endl;

        string stateId = num2str(st->id);
        int remSpaces = stId.size() - stateId.size() - 1;
        cout << " " << stateId;
        while(remSpaces--)
            cout << " ";

        string acc = "           ";
        if(st->isAcceptanceState())
            acc = st->name;

        remSpaces = accNameSep.size() - acc.size() - 1;
        cout << "| " << acc;

        while(remSpaces--)
            cout << " ";

        for(char c: allTransitions){
            if(st->isValidTransition(c)){
                string stId = num2str(st->nextState(c)->id);
                cout << "| ";
                int remSeps = colSep.size() - stId.size() - 2;
                cout << stId;
                while(remSeps--)
                    cout << " ";
            }
            else{
                cout << "|   -   ";
            }
        }
        cout << endl;
    }

    if(fileName != "STDOUT"){
        std::cout.rdbuf(backup);
        out.close();
    }
}

string Automata::num2str ( int Number )
  {
     ostringstream ss;
     ss << Number;
     return ss.str();
  }
