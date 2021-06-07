#ifndef __VORGABE_H__
#define __VORGABE_H__

#include <semaphore.h>
#include <stdbool.h>

#define TOOL_COUNT 7                        // Anzahl der verschiedenen Geräte.
#define ARCHAEOLOGIST_COUNT 4               // Anzahl der Archäologen
#define EXCAVATIONS_PER_ARCHAEOLOGIST 2     // Anzahl der Ausgrabungstätten pro Archäologe

typedef struct ExcavationTool {  // Definition der struct welche die Ausgrabungsgeräte Beschreibt
    char name[256];              // Der Name des Geräts
    int maxCount;                // Die Summe an verfügbaren Geräten dieses Typs
    sem_t sem;                   // Die aktuell verfügbare Anzahl von diesem Gerät
} ExcavationTool;


typedef struct ExcavationStep { // Dieser struct beschreibt einen Ausgrabungschritt
    int toolIndex;              // Der Index für das Array 'tools' welches Gerät benötigt wird
    int time;                   // Gibt in Sekunden an wie lange mit den Geräten gearbeitet werden soll
    bool finished;              // Falls true ist die Ausgrabung nach diesem Schritt beendet
} ExcavationStep;


extern ExcavationTool tools[TOOL_COUNT]; // Dieses Array beschreibt die verfügbaren Geräte im Institut

ExcavationStep get_next_step(); // Beschreibt den nächsten Ausgrabungschritt

extern pthread_mutex_t lock; // Mit diesem Mutex kann der Geräteraum reserviert werden

extern pthread_t archaeologists[ARCHAEOLOGIST_COUNT]; // pthreads welche die Archäologen simulieren

// a3_a.c / a3_b.c / a3_c.c

void *work(void *arg); // Diese Funktion soll die Ausgrabungen simulieren. Diese müsst ihr implementieren!

#endif
