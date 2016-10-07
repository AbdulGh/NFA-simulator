#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "fileToNFA.h"
#include "state.h"
#include "nfa.h"

#define MAXLEN 7

state* getState(char* id);

NFA* createNFA();

void initSTable();
void clearSTable();

//returns the number of chars read
int readID(FILE* fp, int maxlen, char* buf);
//returns the first nonalphanum char
int flushWhiteSpace(FILE* fp);

NFA* fileToNFA(char* filename)
{
	FILE *fp = fopen(filename, "r");

	if (fp == NULL)
	{
		perror("Could not open the file\n");
		return NULL;
	}

	initSTable();

	char* buf = calloc(MAXLEN + 1, sizeof(char));

	int c;
	while ((c = flushWhiteSpace(fp)) != EOF)
	{
		//check if this is an accepting state
		int accepting = 0;
		if (c == '*')
		{
			accepting = 1;
		}
		else ungetc(c, fp);

		readID(fp, MAXLEN, buf);
		state* s = getState(buf);
		s->accepting = accepting;

		int transRead = 0;
		while (!transRead)
		{
			//see which character this trans is for
			int tchar = flushWhiteSpace(fp);

			if (tchar == ':') tchar = EPSILLON;
			else if (tchar == '-' || tchar == EOF)
			{
				transRead = 1;
				continue;
			}

			//start reading transitions
			while ((c = readID(fp, MAXLEN, buf))) 
			{
				if (strlen(buf) == 0) continue;
				state* to = getState(buf);
				addTransition(s, to, tchar);
				if (c == ';') break;
			}
		}
	}

	NFA* done = createNFA();

	clearSTable();
	free(buf);
	fclose(fp);

	return done;
}

int readID(FILE* fp, int maxlen, char* buf)
{
	int c = flushWhiteSpace(fp);
	if (c == EOF) return 0;

	//read name into buffer
	int i;
	for (i = 0; i < maxlen && isalnum(c); i++)
	{
		buf[i] = c;
		c = fgetc(fp);
	}
	buf[i] = '\0';

	//throw away rest of the name
	while (isalnum(c) && c != ';') c = fgetc(fp);

	return c;
}

int flushWhiteSpace(FILE* fp)
{
	int c;
	while (isspace(c = fgetc(fp)) && c != EOF);
	return c; 
}

//'symbol table'
typedef struct Token token;
struct Token
{
	char* name;
	state* s;
};

token** table;
int tablepointer;
int chunks;

#define CHUNKSIZE 5

state* getState(char* id)
{
	for (int i = 0; i < tablepointer; i++)
	{
		if (strcmp(table[i]->name, id) == 0)
		{
			return table[i]->s;
		}
	}

	if (tablepointer >= chunks * CHUNKSIZE)
	{
		table = realloc(table, ++chunks * CHUNKSIZE * sizeof(token*));
	}

	token* newToken = malloc(sizeof(token));
	newToken->name = calloc(strlen(id) + 1, sizeof(char));
	strcpy(newToken->name, id);
	newToken->s = newState();
	table[tablepointer++] = newToken;
	return newToken->s;
}

void deleteToken(token* bye)
{
	free(bye->name);
	free(bye);
}

NFA* createNFA()
{
	if (tablepointer == 0) return NULL;

	NFA* finished = newNFA();
	finished->startState = table[0]->s;

	for (int i = 0; i < tablepointer; i++)
	{
		addStateToNFA(finished, table[i]->s);
	}

	return finished;
}

void initSTable()
{
	tablepointer = 0;
	chunks = 1;
	table = malloc(chunks * CHUNKSIZE * sizeof(token*));
}

void clearSTable()
{
	for (int i = 0; i < tablepointer; i++)
	{
		deleteToken(table[i]);
	}
	free(table);

	chunks = 0;
	tablepointer = 0;
}