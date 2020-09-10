#ifndef _CHAR_VECTOR_
#define _CHAR_VECTOR_

/**
 * @brief: A struct for a dynammic string array
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

/* Vector Mode */

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

/* Linked List Mode */

/**
 * @brief: Replace element with first NULL in CVector 
 * @params: CVector *cvector: CVector to add element to
 *          char *element: element to add 
 */
void add_to_CVector(CVector *cvector, char *element);

/**
 * @brief: Remove element and replace with NULL in CVector and
 *         returns the related string
 * @params: CVector *cvector: CVector to remove element from
 *          char *element: element to remove
 *          bool del: if false: doesnt replace by null
 * @return: Returns a freeable string corresponding to the pid
 */
char *remove_from_CVector(CVector *cvector, pid_t pid, bool del);

#endif
