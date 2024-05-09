#include <stdio.h>
#include <stdlib.h>
#include "race.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s nombre_de_voitures\n", argv[0]);
        return 1;
    }

    int nbVoitures = atoi(argv[1]);
    setup_race(nbVoitures);
    start_race(nbVoitures);
    cleanup_race();
    return 0;
}
