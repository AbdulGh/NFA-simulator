#include <stdlib.h>
#include <stdio.h>

#include "state.h"

node* newNode(state* s)
{
    node* n = malloc(sizeof(node));
    n->data = s;
    n->next = NULL;
    return n;
}

state* newState()
{
    state* newState = malloc(sizeof(state));
    newState->accepting = NOT_ACCEPTING;

    //transitions for ASCII values 32-126 and the empty string, initialised to null
    newState->transitionLists = calloc(95, sizeof(node));

    return newState;
}

node* transition(state* old, char c)
{
    if (!(c >= 32 && c <= 127)) return NULL;
    return old->transitionLists[c - 32];
}

void addTransition(state* from, state* to, char c)
{
    if (!(c >= 32 && c <= 127)) return;

    node* currentHead = transition(from, c);

    if (currentHead == NULL)
    {
        from->transitionLists[c - 32] = newNode(to);
    }

    else
    {
        //check it's not already in the linked list
        node* it = currentHead;
        while (it->next != NULL)
        {
            if (it->data == to) return;
            else it = it->next;
        }

        node* n = newNode(to);
        it->next = n;
    }
}

void freeList(node* start, int deleteStates)
{
    node* it = start;
    while (it != NULL)
    {
        if (deleteStates)
        {
            deleteState(it->data);
        }
        node* temp = it;
        it = it->next;
        free(temp);
    }
}

void deleteState(state *s)
{
    for (int i = 0; i < 96; i++)
    {
        freeList(s->transitionLists[i], 0);
    }

    free(s->transitionLists);
    free(s);
}