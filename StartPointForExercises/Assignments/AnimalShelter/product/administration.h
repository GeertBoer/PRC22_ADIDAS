#ifndef _ADMINISTRATION_H
#define _ADMINISTRATION_H

#include "animal.h"

int addAnimal(const ANIMAL* animalPtr, ANIMAL* animalArray, int animalArrayLength, int* newAnimalArrayLength);
/* pre    : animalArray has enough space for 1 more animal
 * post   : animalArray is extended with the information from animalPtr, the updated array length is passed in newAnimalArrayLength
 * returns: 0 on success or -1 if an error occurs
 */

int removeAnimal(const char* name, ANIMAL* animalArray, int animalArrayLength, int* newAnimalArrayLength);
/* pre    : 
 * post   : All animals with name 'name' are removed from AnimalArray, the updated array length is passed in newAnimalArrayLength
 * returns: The number of removed animals, 0 if no animals removed or -1 if an error occurs
 */

int sortAnimalsByAge(ANIMAL* animalArray, int animalArrayLength);
/* pre    : 
 * post   : All animals in animalArray are sorted by age
 * returns: 0 on success or -1 if an error occurs
 */

int sortAnimalsByYearFound(ANIMAL* animalArray, int animalArrayLength);
/* pre    : 
 * post   : All animals in animalArray are sorted by the year in which they were found
 * returns: 0 on success or -1 if an error occurs
 */

int sortAnimalsBySex(ANIMAL *animalArray, int animalArrayLength);
/* pre    :
 * post   : All animals in animalArray are sorted: first female animals and then male animals
 * returns: 0 on success or -1 if an error occurs
 */

int findAnimalByName(const char* name, const ANIMAL* animalArray, int animalArrayLength, ANIMAL* animalPtr);
/* pre    : 
 * post   : The first animal from animalArray with name 'name' is copied into animalPtr
 * returns: 1 on success, 0 if not found or -1 if an error occurs
 */

#endif
