#ifndef _ANIMAL_H
#define _ANIMAL_H

typedef enum
{
    Cat,
    Dog,
    GuineaPig,
    Parrot
} SPECIES;

typedef enum
{
    Male,
    Female
} SEX;

typedef struct
{
    int Day;
    int Month;
    int Year;
} DATE;

#define MaxNameLength 25
#define MaxLocationLength 100
typedef struct
{
    char    Name[MaxNameLength];
    SPECIES Species;
    SEX     Sex;
    int     Age;
    char    FoundAtLocation[MaxLocationLength];
    DATE    DateFound;
} ANIMAL;

#endif
