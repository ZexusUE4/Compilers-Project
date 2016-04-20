#include "state_base.h"

/* Initialize the idCounter static member of the StateBase class*/
int state_base::id_counter = 1;

state_base::state_base(){
	id = id_counter++;
}

state_base::~state_base(){

}

void state_base::set_acceptance_state(string Keyword){
	name = Keyword;
	acceptance_flag = true;
}

bool state_base::is_acceptance_state(){
	return acceptance_flag;
}
