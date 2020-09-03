#ifndef _CHAR_VECTOR_
#define _CHAR_VECTOR_

/**
 * @brief: A dynammic array
 */
typedef struct {
    char **vector;
    size_t used;
    size_t size;
} CVector;

/**
 * @brief: Initialises a CVector
 * @params: CVector *cvector: CVector to initialise
 */
void initCVector(CVector *cvector);

/**
 * @brief: Push an element at end of CVector
 * @params: CVector *cvector: CVector to add element to
 *          char *element: Element to push
 */
void pbCVector(CVector *cvector, char *element);

/**
 * @brief: Free a CVector
 * @params: CVector *cvector: CVector to free
 */
void freeCVector(CVector *cvector);

/**
 * @brief: Converts a string to a CVector with 
 *         delimiter as "\t" and " "
 * @params: char *str: string to convert
 * @return: Pointer to a CVector
 */
CVector *to_args(char *str);

#endif
