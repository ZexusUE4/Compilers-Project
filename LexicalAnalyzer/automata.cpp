#include "automata.h"
#include <queue>
#include <iostream>
#include <fstream>
#include <sstream>

automata::automata(){

}

automata::automata(state* startState, EAutomataType Type){

	this->start_state = startState;
	automata_type = Type;

}

automata::~automata(){

	queue<state*> q;
	set<state*> visited;
	q.push(start_state);
	visited.insert(start_state);

	while (!q.empty()){
		state* top = q.front(); q.pop();
		vector<char> validTransitions = top->get_valid_transtitions();

		for (char c : validTransitions){
			state* neigh = top->next_state(c);

			if (visited.find(neigh) == visited.end()){
				visited.insert(neigh);
				q.push(neigh);
			}

		}
		delete top;
	}

}

void automata::print_automata(){


	set<state*> visited;
	queue<state*> q;

	q.push(start_state);
	visited.insert(start_state);

	while (!q.empty()){
		state* top = q.front();
		q.pop();

		top->print_state(true);

		vector<char> validTransitions = top->get_valid_transtitions();

		for (char c : validTransitions){
			vector<state*> next_states = top->next_states(c);

			cout << c << "  -->  ";

			for (state* st : next_states){
				st->print_state();

				if (visited.find(st) == visited.end()){
					visited.insert(st);
					q.push(st);
				}
			}
			cout << endl;
		}
	}
}

size_t automata::count_states(){

	set<state*> visited;
	queue<state*> q;
    q.push(start_state);
    visited.insert(start_state);

    while(!q.empty()){
        state* top = q.front(); q.pop();
        vector<char> validTransitions = top->get_valid_transtitions();

        for(char c : validTransitions){
			vector<state*> next_states = top->next_states(c);

            for(state* st: next_states){
                if(visited.find(st) == visited.end()){
                    visited.insert(st);
                    q.push(st);
                }
            }
        }
    }

    return visited.size();
}

vector<state*> automata::get_all_states(){

	set<state*> visited;
	queue<state*> q;
    q.push(start_state);
    visited.insert(start_state);

    while(!q.empty()){
        state* top = q.front(); q.pop();
        vector<char> validTransitions = top->get_valid_transtitions();

        for(char c : validTransitions){
			vector<state*> next_states = top->next_states(c);

            for(state* st: next_states){
                if(visited.find(st) == visited.end()){
                    visited.insert(st);
                    q.push(st);
                }
            }
        }
    }

	return vector<state*>(visited.begin(), visited.end());

}

vector<char> automata::get_all_transtions(){

    set<char> allTransitions;

	vector<state*> allStates = get_all_states();

    for(state * st: allStates){
        vector<char> validTransitions = st->get_valid_transtitions();
        allTransitions.insert(validTransitions.begin(),validTransitions.end());
    }

    return vector<char>(allTransitions.begin(),allTransitions.end());
}

void automata::print_transition_table(string fileName){
    streambuf *backup;
    backup = std::cout.rdbuf();     // back up cout's streambuf
    std::ofstream out;

    if(fileName != "STDOUT"){
        out.open(fileName);
        std::streambuf *coutbuf = std::cout.rdbuf(); //save old buf
        std::cout.rdbuf(out.rdbuf()); //redirect std::cout to out.txt
    }


	vector<state*> allStates = get_all_states();
    vector<char> allTransitions = get_all_transtions();

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

    for(state* st: allStates){
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
        if(st->is_acceptance_state())
            acc = st->name;

        remSpaces = accNameSep.size() - acc.size() - 1;
        cout << "| " << acc;

        while(remSpaces--)
            cout << " ";

        for(char c: allTransitions){
            if(st->is_valid_transition(c)){
                string stId = num2str(st->next_state(c)->id);
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

string automata::num2str ( int Number )
  {
     ostringstream ss;
     ss << Number;
     return ss.str();
  }
