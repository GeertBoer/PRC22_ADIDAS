/* **********************************************
 * generic linked list in plain c
 * author: Freddy Hurkmans
 * date: dec 20, 2014
 * **********************************************/

/* ******************************************************************************
 * This list is designed using object oriented ideas, but is implemented in c.
 * 
 * The idea is: you construct a list for a certain data structure (say: mystruct):
 * list_admin* mylist = construct_list(sizeof(mystruct))
 * mylist contains private data for this list implementation. You need not worry
 * about it, only give it as first parameter to each list method.
 * 
 * This means you can have multiple lists at the same time, just make sure you
 * give the right list_admin structure to the list function.
 * When you're done with your list, just call the destructor: destruct_list(&mylist)
 * The destructor automatically deletes remaining list elements and the list administration.
 * 
 * As this list implementation keeps its own administration in list_admin, you need
 * not worry about next pointers and such. Your structure doesn't even need pointers
 * to itself, you only need to worry about data. A valid struct could thus be:
 * typedef struct
 * {
 *     int  nr;
 *     char text[100];   
 * } mystruct;
 * 
 * Adding data to the list is done using list_add_* methods, such as list_add_tail,
 * which adds data to the end of the list:
 * mystruct data = {10, "hello world!"};
 * int result = list_add_tail(mylist, &data);
 * You don't have to provide the size of your struct, you've already done that in
 * the destructor. If result < 0 list_add_* has failed (either a parameter problem
 * or malloc didn't give memory).
 * 
 * You can get a pointer to your data using list_get_*, e.g. get the 2nd element:
 * mystruct* dataptr = list_get_element(admin, 1);
 * or get the last element:
 * mystruct* dataptr = list_get_last(admin);
 * If dataptr is not NULL it points to the correct data, else the operation failed.
 * 
 * Searching for data in your list can be done with list_index_of*. list_index_of
 * compares the data in each list item to the given data. If they are the same, it
 * returns the index. If you want to search for an element with a certain value for
 * nr or text (see mystruct) you can implement your own compare function and use
 * list_index_of_special (check memcmp for required return values):
 * int mycompare(void* p1, void* p2, size_t size)
 * {
 *     // this example doesn't need size!
 *     mystruct* org = (mystruct*)p1;
 *     int* nr = (int*)p2;
 *     return org->nr - nr; // return 0 if they are the same
 * }
 * Say you want to search for an element that has nr 10:
 * int nr = 10;
 * int index = list_index_of_special(mylist, &nr, mycompare);
 * As noted earlier: mycompare must have the same prototype as memcmp. In fact:
 * list_index_of is a shortcut for list_index_of_special(mylist, data, memcmp).
 * 
 * Finally you can delete items with list_delete_*. They should work rather
 * straight forward. If they succeed they return 0. You don't have to delete
 * all items before destructing your list.
 * 
 * ******************************************************************************/


#include <string.h> /* for memcmp and memcpy */

#include "list.h"

static size_t data_size(const list_admin* admin);
static list_head* get_list_head_of_element_n(list_admin* admin, int index);
static void remove_first_element(list_admin* admin);
static int remove_non_first_element(list_admin* admin, int index);

/* **********************************************
 * Constructor / destructor
 * **********************************************/
list_admin* construct_list(size_t element_size)
{
    list_admin* admin = malloc(sizeof(*admin));
    if (admin != NULL)
    {
        admin->head = NULL;
        admin->element_size = element_size;
        admin->nr_elements = 0;
    }
    return admin;
}

void destruct_list(list_admin** admin)
{
    if ((admin != NULL) && (*admin != NULL))
    {
        while ((*admin)->head != NULL)
        {
            list_head* temp = (*admin)->head;
            (*admin)->head = (*admin)->head->next;
            free(temp);
            temp = NULL;
        }
        free(*admin);
        *admin = NULL;
    }
}

/* **********************************************
 * Public functions
 * **********************************************/
int list_get_nr_elements(list_admin* admin)
{
    int nr_elements = -1;
    if (admin != NULL)
    {
        nr_elements = admin->nr_elements;
    }
    return nr_elements;
}

int list_add_tail(list_admin* admin, const void* data)
{
    int result = -1;
    if ((admin != NULL) && (data != NULL))
    {
        list_head* new = malloc(data_size(admin));
        if (new != NULL)
        {
            result = 0;
            new->next = NULL;
            memcpy(new+1, data, admin->element_size);

            if (admin->head == NULL)
            {
                admin->head = new;
            }
            else
            {
                list_head* temp = get_list_head_of_element_n(admin, admin->nr_elements-1);
                temp->next = new;
            }

            admin->nr_elements++;
        }
    }
    return result;
}

void* list_get_element(list_admin* admin, int index)
{
    list_head* item = get_list_head_of_element_n(admin, index);
    if(item != NULL)
    {
        item++; // user data is just beyond list_head, thus we must increase the pointer
    }
    return item;
}

void* list_get_last(list_admin* admin)
{
    list_head* item = NULL;
    if (admin != NULL)
    {
        item = list_get_element(admin, admin->nr_elements-1);
    }
    return item;
}

int list_index_of(list_admin* admin, const void* data)
{
    return list_index_of_special(admin, data, memcmp);
}

int list_index_of_special(list_admin* admin, const void* data, CompareFuncPtr compare)
{
    int index = -1;
    if ((admin != NULL) && (data != NULL) && (compare != NULL))
    {
        list_head* temp = admin->head;
        index = 0;
        while ((temp != NULL) && (compare(temp+1, data, admin->element_size) != 0))
        {
            temp = temp->next;
            index++;
        }
        if (temp == NULL)
        {
            index = -1;
        }
    }
    return index;
}

int list_delete_item(list_admin* admin, int index)
{
    int result = -1;
    if ((admin != NULL) && (index >= 0) && (admin->head != NULL) && (index < admin->nr_elements))
    {
        if (index == 0)
        {
            result = 0;
            remove_first_element(admin);
        }
        else
        {
            result = remove_non_first_element(admin, index);
        }
    }
    return result;    
}

int list_delete_last(list_admin* admin)
{
    int result = -1;
    if (admin != NULL)
    {
        result = list_delete_item(admin, admin->nr_elements - 1);
    }
    return result;
}

/* **********************************************
 * Private functions
 * **********************************************/
static size_t data_size(const list_admin* admin)
{
    return admin->element_size + sizeof(list_head);
}

static list_head* get_list_head_of_element_n(list_admin* admin, int index)
{
    list_head* item = NULL;
    if ((admin != NULL) && (index >= 0) && (index < admin->nr_elements))
    {
        item = admin->head;
        for (int i = 0; (i < index) && (item != NULL); i++)
        {
            item = item->next;
        }
    }
    return item;
}

static void remove_first_element(list_admin* admin)
{
    list_head* temp = admin->head;
    admin->head = admin->head->next;
    free(temp);
    temp = NULL;
    admin->nr_elements--;
}

static int remove_non_first_element(list_admin* admin, int index)
{
    int result = -1;
    if (index > 0)
    {
        list_head* temp = get_list_head_of_element_n(admin, index-1);
        if ((temp != NULL) && (temp->next != NULL)) // to remove element n, element n-1 and n must exist
        {
            result = 0;
            list_head* to_delete = temp->next;
            temp->next = to_delete->next;
            free(to_delete);
            to_delete = NULL;
            admin->nr_elements--;
        }
    }
    return result;
}
