#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "nfa.h"

NFA* newNFA()
{
	NFA* new = malloc(sizeof(NFA));
	new->stateList = NULL;
	new->startState = NULL;
	return new;
}

void deleteNFA(NFA* bye, int deleteStates)
{
	freeList(bye->stateList, deleteStates);
	free(bye);
}

//returns the number of elements added
int addToLL(node** head, state* new)
{
	if (*head == NULL)
	{
		*head = newNode(new);
	}

	else
	{
		node* it = *head;
		while (it->next != NULL)
		{
			if (it->data == new) return 0;
			else it = it->next;
		}
		it->next = newNode(new);

	}

	return 1;
}

int countLL(node* n)
{
	int count = 0;

	while (n)
	{
		count++;
		n = n->next;
	}

	return count;
}

int LLintersection(node** headDest, node* headSrc)
{

	int additions = 0;
	node* it = headSrc;
	while (it != NULL)
	{
		additions += addToLL(headDest, it->data); //would be nice to just link them together, but we shouldn't allow duplicates
		it = it->next;
	}
	return additions;
}

void addStateToNFA(NFA* nfa, state* new)
{
	addToLL(&nfa->stateList, new);
}

void eclose(state* s, node** intersect)
{
	if (addToLL(intersect, s))
	{
		node* it = transition(s, EPSILLON);
		while (it != NULL)
		{
			eclose(it->data, intersect);
			it = it->next;
		}
	}
}

int simulate(NFA* nfa, char* string)
{
	node* currentStates = NULL;
	eclose(nfa->startState, &currentStates);

	for (int i = 0; i < strlen(string); i++)
	{
		node* nextStates = NULL;

		node* it = currentStates;

		if (it == NULL) return 0; //NFA died
		
		while (it != NULL) //get the intersection of all the state transitions
		{
			node* it2 = transition(it->data, string[i]);
			while (it2 != NULL)
			{
				node* foundStates = NULL;
				eclose(it2->data, &foundStates);
				LLintersection(&nextStates, foundStates);
				freeList(foundStates, 0);
				it2 = it2->next;
			}
			it = it->next;
		}

		freeList(currentStates, 0);
		currentStates = nextStates;
	}

	//check if any current states are accepting at the end of the string
	int accepting = 0;
	node* it = currentStates;

	while (it != NULL)
	{
		if (it->data->accepting)
		{
			accepting = 1;
			break;
		}
		it = it->next;
	}

	freeList(currentStates, 0);
	return accepting;
}

