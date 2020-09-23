#include "libs.h"

static size_t initSize = 2;

void initCVector(CVector *cvector) {

    cvector->vector = malloc(initSize * sizeof(char *));

    if (cvector->vector == NULL) {
        // Throw fatal error
        fatal_error_check(0, 0);
    }

    cvector->vector[0] = NULL;
    cvector->used = 0;
    cvector->size = initSize;
}

static void increase_size(CVector *cvector) {
    cvector->size *= 2;

    cvector->vector = realloc(cvector->vector, 
            cvector->size * sizeof(char *));

    if (cvector->vector == NULL) { 
        // Throw fatal error
        fatal_error_check(0, 0);
    }

    for (int i = cvector->used; i < cvector->size; i++) {
        cvector->vector[i] = NULL;
    }
}

static void decrease_size(CVector *cvector) {
   char **new_vector = malloc(cvector->used * sizeof(char *));  

   int current = 0;
   for (int i = 0; i < cvector->used; i++) {
       if (cvector->vector[i]) {
           new_vector[current++] = cvector->vector[i];
       }
   }

   cvector->size = cvector->used;
   free(cvector->vector);
   cvector->vector = new_vector;
}

/* Vector Mode */

void pbCVector(CVector *cvector, char *element) {
    if (cvector->used == cvector->size) {
        increase_size(cvector);
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
