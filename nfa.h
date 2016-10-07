#ifndef NFA_INCLUDED
#define NFA_INCLUDED

#include "state.h"

typedef struct NondeterministicFiniteAutomaton NFA;
struct NondeterministicFiniteAutomaton
{
	state* startState;

	//list of pointers to states, for deletion
	node* stateList;
};

NFA* newNFA();

void deleteNFA(NFA* bye, int deleteStates);
void addStateToNFA(NFA* nfa, state* new);

int simulate(NFA* nfa, char* string);

#endif