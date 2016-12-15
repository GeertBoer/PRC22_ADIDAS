/*
 * animal_shelter.c
 *
 *  Created on: August 31, 2016
 *      Author: Freddy Hurkmans
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#include "resource_detector.h"
#include "administration.h"
#include "file_element.h"
#include "animal.h"
#include "terminal_io.h"

const char *filename = "testFile";

static void addAnimalToAdministration(ANIMAL* animals, int* nrAnimals)
{
    // this function does not test the validity of input parameters as it is
    // a static function (equivalent to private in a C# class). Please do not
    // confuse this use of static with a static method in C#!
    ANIMAL animal = {"", 0, 0, 0, "", {0, 0, 0}};

    getStr("enter animal name: ", animal.Name, MaxNameLength);
    animal.Species = getSpecies();
    animal.Sex = getSex();
    animal.Age = getInt("enter age: ");
    getStr("enter found location: ", animal.FoundAtLocation, MaxLocationLength);
    animal.DateFound = getDate("enter found date:\n");

    int result = addAnimal(&animal, animals, *nrAnimals, nrAnimals);
    printf("\nAdd animal result: %s\n", result == 0 ? "success": "fail");
}

static void removeAnimalFromAdministration(ANIMAL* animals, int* nrAnimals)
{
    char animalName[MaxNameLength] = "";
    getStr("enter animal name: ", animalName, MaxNameLength);
    
    int result = removeAnimal(animalName, animals, *nrAnimals, nrAnimals);
    printf("%d animals removed (-1 is error)\n", result);
}

static void findAndPrintAnimal(const ANIMAL* animals, int nrAnimals)
{
    ANIMAL animal = {"", 0, 0, 0, "", {0, 0, 0}};
    char animalName[MaxNameLength] = "";

    getStr("enter animal name: ", animalName, MaxNameLength);
    
    int result = findAnimalByName(animalName, animals, nrAnimals, &animal);
    if (result == 1)
    {
        printAnimals(&animal, 1);
    }
    else if (result == 0)
    {
        printf("Find animal result: animal %s not found\n", animalName);
    }
    else
    {
        printf("Find animal result: fail\n");
    }
}

static void addTestData(ANIMAL* animals, int* nrAnimals)
{
    ANIMAL a1 = {"Max", Dog, Male, 12, "Schoolstraat 1", {1, 2, 3}};
    ANIMAL a2 = {"Kiekerjan", Cat, Female, 4, "boven in boom", {4, 3, 2}};
    ANIMAL a3 = {"Lorre", Parrot, Male, 40, "werk", {8, 9, 10}};
    ANIMAL a4 = {"Fifi", Dog, Female, 1, "Kerkstraat 13", {1, 1, 100}};
    ANIMAL a5 = {"Piep", GuineaPig, Male, 3, "thuis", {3, 4, 1}};

    animals[(*nrAnimals)++] = a1;
    animals[(*nrAnimals)++] = a2;
    animals[(*nrAnimals)++] = a3;
    animals[(*nrAnimals)++] = a4;
    animals[(*nrAnimals)++] = a5;
}

int main (int argc, char * argv[])
{
    const int MaxNrAnimals = 20;
    // char filename[MAX_STRLEN] = "";
    ANIMAL animals[MaxNrAnimals];
    int nrAnimals = 0;
    MenuOptions choice = MO_ShowAnimals;

    addTestData(animals, &nrAnimals);

    printf ("PRC assignment 'Animal Shelter' (version 1)\n"
            "-------------------------------------------");

    if (argc != 1)
    {
        fprintf (stderr, "%s: argc=%d\n", argv[0], argc);
    }

    while (choice != MO_Quit)
    {
        printf ("\n\nMENU\n====\n\n");
        choice = getMenuChoice();

        switch (choice)
        {
            // case MO_SelectFile:
            //     getStr("enter administration filename: ", filename, MAX_STRLEN);
            //     break;
            case MO_ShowAnimals:
                printAnimals(animals, nrAnimals);
                break;
            case MO_AddAnimal:
                addAnimalToAdministration(animals, &nrAnimals);
                break;
            case MO_RemoveAnimal:
                removeAnimalFromAdministration(animals, &nrAnimals);
                break;
            case MO_SortAnimalsByAge:
                {
                    int result = sortAnimalsByAge(animals, nrAnimals);
                    printf("Sort animal by age result: %s\n", result == 0 ? "success": "fail");
                }
                break;
            case MO_SortAnimalsByYearFound:
                {
                    int result = sortAnimalsByYearFound(animals, nrAnimals);
                    printf("Sort animal by year found result: %s\n", result == 0 ? "success": "fail");
                }
                break;
            case MO_SortAnimalsBySex:
                {
                    int result = sortAnimalsBySex(animals, nrAnimals);
                    printf("Sort animal by sex result: %s\n", result == 0 ? "success": "fail");
                }
                break;
            case MO_FindAnimal:
                findAndPrintAnimal(animals, nrAnimals);
                break;
	    case MO_Save:
		{
			int result = writeAnimals(filename, animals, nrAnimals);
			printf("Save animals to file result %s\n", result == 0 ? "success" : "fail");
		}
		break;
	    case MO_Load:
		{
			nrAnimals = getNrAnimalsInFile(filename);
			int result = -1;
			if(nrAnimals != -1)
			{
				result = readAnimals(filename, animals, nrAnimals);
			}
			printf("Load animals from file result %s\n", result != -1 ? "success" : "fail");
		}
		break;
            case MO_Quit:
                // nothing to do here
                break;
            default:
                printf ("ERROR: invalid choice: %d\n", choice);
                break;
        }
    }
    return 0;
}

