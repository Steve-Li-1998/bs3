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
    int currentToolsIndex[EXCAVATIONS_PER_ARCHAEOLOGIST];
    for (int i = 0; i < EXCAVATIONS_PER_ARCHAEOLOGIST; ++i) {
        ExcavationStep excavationStep = get_next_step();
        if (sem_wait(&tools[excavationStep.toolIndex].sem)){
            fprintf(stderr, "sem_wait failed");
            exit(1);
        }
        currentToolsIndex[i] = excavationStep.toolIndex;
        sleep(excavationStep.time);
        if (excavationStep.finished){
            if(sem_post(&tools[excavationStep.toolIndex].sem)){
                fprintf(stderr, "sem_post failed");
                exit(1);
            }
            currentToolsIndex[i] = TOOL_COUNT;
            //da [0...TOOL_COUNT-1] schon besitzt werden. wenn index TOOL_COUNT ist, bedeutet das Werkzeug zurueckgegeben wird
        }
    }
    for (int i = 0; i < EXCAVATIONS_PER_ARCHAEOLOGIST; ++i) {
        if(currentToolsIndex[i]<TOOL_COUNT){
            if(sem_post(&tools[excavationStep.toolIndex].sem)){
                fprintf(stderr, "sem_post failed");
                exit(1);
            }
            }
        }
    }


	printf("Alle Ausgrabungen von [A%d] sind fertig\n", my_num);
	return NULL;
}
