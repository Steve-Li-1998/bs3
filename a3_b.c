#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
// Zusätzliche Header-Dateien hier!

#include "vorgabe.h"

void *work(void *arg)
{
	int my_num = *((int *)arg);

	printf("[A%d] Ich bin bereit zu Graben!\n", my_num);

	// *** HIER EUER CODE ***


	printf("Alle Ausgrabungen von [A%d] sind fertig\n", my_num);
	return NULL;
}
