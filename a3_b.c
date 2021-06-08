#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
// Zusätzliche Header-Dateien hier!
#include <time.h>
#include <errno.h>

#include "vorgabe.h"

void *work(void *arg)
{
	int my_num = *((int *)arg);

	printf("[A%d] Ich bin bereit zu Graben!\n", my_num);

	// *** HIER EUER CODE ***
    int currentToolsIndex[EXCAVATIONS_PER_ARCHAEOLOGIST];
    for (int i = 0; i < EXCAVATIONS_PER_ARCHAEOLOGIST; ++i) {
        ExcavationStep excavationStep = get_next_step();
        // pruefen, ob das Werkzeug schon vorhanden
        bool toolAvalible = false;
        for (int j = 0; j < i; ++j) {
            if(currentToolsIndex[j] == excavationStep.toolIndex){
                toolAvalible = true;
                currentToolsIndex[i] = currentToolsIndex[j];
                currentToolsIndex[j] = TOOL_COUNT;
                break;
            }
        }
        if(!toolAvalible){
            struct timespec waittime;
            if(clock_gettime(CLOCK_REALTIME, &waittime)) {
                perror("clock_gettime");
                exit(1);
            }
            waittime.tv_sec += 10;
            if (sem_timedwait(&tools[excavationStep.toolIndex].sem, &waittime)){
                if(errno==ETIMEDOUT) {
                    --i;
                    // Die erfolglose Ausgrabung soll nicht zu den erledigten Ausgrabungen zaehlen.
                    for (int j = 0; j < i; ++j) {
                        sem_post(&tools[currentToolsIndex[j]].sem);
                        currentToolsIndex[j] = TOOL_COUNT;
                        sleep(1);
                    }
                }
                else {
                    fprintf(stderr, "sem_wait failed");
                    exit(1);
                }
            }
            currentToolsIndex[i] = excavationStep.toolIndex;
        }
        sleep(excavationStep.time);
        if (excavationStep.finished){
            if(sem_post(&tools[currentToolsIndex[i]].sem)){
                fprintf(stderr, "sem_post failed");
                exit(1);
            }
            currentToolsIndex[i] = TOOL_COUNT;
            //da [0...TOOL_COUNT-1] schon besitzt werden. wenn index TOOL_COUNT ist, bedeutet das Werkzeug zurueckgegeben wird
        }
    }
    for (int i = 0; i < EXCAVATIONS_PER_ARCHAEOLOGIST; ++i) {
        if(currentToolsIndex[i]<TOOL_COUNT){
            if(sem_post(&tools[currentToolsIndex[i]].sem)){
                fprintf(stderr, "sem_post failed");
                exit(1);
            }
            currentToolsIndex[i] = TOOL_COUNT;
        }
    }



	printf("Alle Ausgrabungen von [A%d] sind fertig\n", my_num);
	return NULL;
}
