#ifndef STATE_H_INCLUDED
#define STATE_H_INCLUDED

#define ACCEPTING 0
#define NOT_ACCEPTING 1
#define EPSILLON 127

typedef struct State state;
typedef struct Node node;
struct State
{
    int accepting;
    node** transitionLists;
};

//used to make a linked list of states for a NFA transition
struct Node
{
    state* data;
    node* next;
};

node* newNode(state* s);

state* newState();
node* transition(state* old, char c);
void addTransition(state* from, state* to, char c);

void deleteState(state* s);
int freeList(node* head, int deleteStates);

#endif
