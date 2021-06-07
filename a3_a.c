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
    for (int i = 0; i < EXCAVATIONS_PER_ARCHAEOLOGIST; ++i) {
        ExcavationStep excavationStep = get_next_step();
        sem_wait(&tools[excavationStep.toolIndex].sem);
        sleep(excavationStep.time);
        if (excavationStep.finished){
            sem_post(&tools[excavationStep.toolIndex].sem);
        }
    }


	printf("Alle Ausgrabungen von [A%d] sind fertig\n", my_num);
	return NULL;
}
