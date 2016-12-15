#pragma once

#include <stdlib.h>

typedef struct list_head
{
    struct list_head* next;
} list_head;

typedef struct
{
    struct list_head* head;
    size_t            element_size;
    size_t            nr_elements;
} list_admin;

typedef int (*CompareFuncPtr)(const void*, const void*, size_t);

// call the constructor before using the list:
// list_admin* mylist = construct_list(sizeof(mystruct));
// if mylist equals NULL, no memory could be allocated. Please don't
// mess with the admin data, this can corrupt your data.
list_admin* construct_list(size_t element_size);

// call the destructor when you're done, pass the list administration
// as referenced parameter (the variable will be set to NULL).
void destruct_list(list_admin** admin);

// Reads the number of elements in your list. Returns -1 in case of
// errors, else the number of elements.
int list_get_nr_elements(list_admin* admin);

// Add data to the end of the list. Returns -1 in case of errors, else 0
int list_add_tail(list_admin* admin, const void* data);

// Returns a pointer to the requested element. Returns NULL in case of
// errors (such as: the element does not exist).
void* list_get_element(list_admin* admin, int index);
void* list_get_last(list_admin* admin);

// Returns the index to the first found list element that's equal to data,
// or -1 in case of errors (such as: not found).
int list_index_of(list_admin* admin, const void* data);

// Returns the index to the first found list element for which cmp says it's,
// equal to data, or -1 in case of errors (such as: not found). cmp must behave
// just like memcmp, i.e.: return 0 when the data matches.
int list_index_of_special(list_admin* admin, const void* data, CompareFuncPtr cmp);

// Delete an item in the list. Returns -1 in case of errors, else 0.
int list_delete_item(list_admin* admin, int index);
int list_delete_last(list_admin* admin);
