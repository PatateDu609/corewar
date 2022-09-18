#include <stdio.h>
#include "op.h"

int nbr_cycles;
int *nbr_champion;
char **champion;

int parsing_args(int argc, char **argv)
{
	for (int i = 0; i < argc; i++)
	{
		if (*argv[i] == "-dump")
			nbr_cycles = argv[i + 1];
		if (*argv[i] == "-n")
		{
			nbr_champion = atoi(*argv[i + 1]);
			champion = *argv[i + 2];
		}
	}
	return (1);
}

int main(int argc, char**argv)
{
	if (argc > CHAMP_MAX_SIZE * 3 + 2 || argc < 2 + 2 * 3)
		printf("Error: bad number of args\n");
	if (!parsing_args(argc, argv))
		printf("Error: bad args\n");
	return (0);
}