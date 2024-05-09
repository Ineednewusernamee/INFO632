// race.h
#ifndef RACE_H
#define RACE_H

typedef struct {
    int id;
    int nbTours;
    int position; 
} Voiture;


void setup_race(int nbVoitures);
void start_race(int nbVoitures);
void cleanup_race();

#endif
