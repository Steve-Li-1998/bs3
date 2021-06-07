#include <time.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>

#include "vorgabe.h"
#include <stdbool.h>

ExcavationTool tools[TOOL_COUNT] = {{"Kamera", 2}, {"Massenspektrometer", 1}, {"Mikroskop", 2}, {"Laserscanner", 1}, 
									{"Bodenradar", 2} , {"Fluxgatemagnetometer", 1}, {"Tachymeter", 1}};
pthread_mutex_t lock;		
pthread_t archaeologists[ARCHAEOLOGIST_COUNT];

ExcavationStep get_next_step() {
	bool finished  = false;
	if(rand() % 5 == 0){
		finished = true;
	}
	return (ExcavationStep){rand() % TOOL_COUNT, (rand() % 4) + 1, finished};
}

int main(void) {
	srand(100);	// Zufallszahlengenerator initialisieren
	
	if (pthread_mutex_init(&lock, NULL)) {
		fprintf(stderr, "pthread_mutex_init failed");
		exit(1);
	}

	// Initialisieren der Semaphore
	for (int i = 0; i < TOOL_COUNT; i++) {
		if (sem_init(&(tools[i].sem), 0, tools[i].maxCount)) {
			fprintf(stderr, "sem_init failed");
			exit(1);
		}
	}
	printf("[ ! ] Semaphore initialisiert.\n");

	// Nummernvariablen zur Übergabe an die Threads anlegen
	int archaeologists_nums[ARCHAEOLOGIST_COUNT];
	for (int i = 0; i < ARCHAEOLOGIST_COUNT; i++) {
		archaeologists_nums[i] = i;
	}

	// Ausgrabungs-Threads anlegen
	for (int i = 0; i < ARCHAEOLOGIST_COUNT; i++) {
		if (pthread_create(&archaeologists[i], NULL, &work, &(archaeologists_nums[i]))){
			fprintf(stderr, "pthread_create failed");
			exit(1);
		}
		struct timespec waittime;
		waittime.tv_nsec = 100L * 1000000L;
		int res;
		do{
			 res = nanosleep(&waittime, &waittime);
		} while(res && errno == EINTR);
	}

	// Ausgrabungs-Threads einsammeln
	for (int i = 0; i < ARCHAEOLOGIST_COUNT; i++) {
		if (pthread_join(archaeologists[i], NULL)) {
			fprintf(stderr, "pthread_join failed");
			exit(1);
		}
	}

	printf("[ ! ] Alle Ausgrabungen beendet.\n");

	if (pthread_mutex_destroy(&lock)) {
		fprintf(stderr, "pthread_mutex_destroy failed");
		exit(1);
	}

	//Zerstören der Semaphore
	for (int i = 0; i < TOOL_COUNT; i++) {
		if (sem_destroy(&(tools[i].sem))) {
			fprintf(stderr, "sem_destroy failed");
			exit(1);
		}
	}

	printf("[ ! ] Semaphore zerstört.\n");
	printf("[!!!] Beende Programm.\n");

	return 0;
}
