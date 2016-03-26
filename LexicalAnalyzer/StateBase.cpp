#include "StateBase.h"
/* Initialize the idCounter static member of the StateBase class*/
int StateBase::idCounter = 1;

StateBase::StateBase(){
	id = idCounter++;
}

void StateBase::setAcceptanceState(string Keyword){
	name = Keyword;
	acceptanceFlag = true;
}

bool StateBase::isAcceptanceState(){
	return acceptanceFlag;
}
