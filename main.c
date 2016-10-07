#include <stdio.h>

#include "nfa.h"
#include "fileToNFA.h"

int main()
{
	NFA* nfa = fileToNFA("examplefile");

	printf("%d\n", simulate(nfa, "xaaaaaaaaaaabbbbbbbbbbbbbbbbbbbbbbbbbbbbbbay"));
	deleteNFA(nfa, 1);
	return 0;
}
