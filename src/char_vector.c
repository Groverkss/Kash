#include "libs.h"

void initCVector(CVector *cvector) {
    size_t initSize = 1;

    cvector->vector = malloc(initSize * sizeof(char *));

    if (cvector->vector == NULL) {
        // Throw fatal error
        fatal_error_check(0, 0);
    }

    cvector->used = 0;
    cvector->size = initSize;
}

void pbCVector(CVector *cvector, char *element) {
    if (cvector->used == cvector->size) {
        cvector->size *= 2;

        cvector->vector = realloc(cvector->vector, 
                cvector->size * sizeof(char *));

        if (cvector->vector == NULL) { 
            // Throw fatal error
            fatal_error_check(0, 0);
        }
    } 

    cvector->vector[cvector->used++] = element;
}

void freeCVector(CVector *cvector) {
    for (int i = 0; i < cvector->used; i++) {
        free(cvector->vector[i]);
        cvector->vector[i] = NULL;
    }

    free(cvector->vector);
    cvector->used = cvector->size = 0;
}

CVector* to_args(char *str) {
    CVector *argv = malloc(sizeof(CVector));
    if (argv == NULL) {
        // Throw fatal error
        fatal_error_check(0, 0);
    }

    initCVector(argv);

    char *save_args;
    char *args = strtok_r(str, " \t", &save_args);

    while(args) {
        char *arg = malloc((strlen(args) + 1) * sizeof(char));
        strcpy(arg, args);

        if (arg == NULL) {
            // Throw fatal error
            fatal_error_check(0, 0);
        }

        pbCVector(argv, arg);

        args = strtok_r(NULL, " \t", &save_args);
    }

    return argv;
}
