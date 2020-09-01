#ifndef _CHAR_VECTOR_
#define _CHAR_VECTOR_

typedef struct {
    char **vector;
    size_t used;
    size_t size;
} CVector;

void initCVector(CVector *cvector);

void pbCVector(CVector *cvector, char *element);

void freeCVector(CVector *cvector);

CVector* to_args(char *str);

#endif
