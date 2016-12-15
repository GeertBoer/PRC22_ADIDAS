#include <string.h> /* for memcmp */

#include "list.h"
#include "unity.h"

// I rather dislike keeping line numbers updated, so I made my own macro to ditch the line number
#define MY_RUN_TEST(func) RUN_TEST(func, 0)

static list_admin* mylist = NULL;
typedef struct
{
    int i;
    int j;
} test_struct;

// compare function that returns 0 if test_struct.i matches, it ignores .j
static int compare_test_func(const void* p1, const void* p2, size_t size)
{
    size = size; // to prevent warnings
    const test_struct* data1 = (const test_struct*)p1;
    const test_struct* data2 = (const test_struct*)p2;
    return data1->i - data2->i;
}
// check n items in list against verify_data
// IMPORTANT: this function must check using list_get_element,
//            else the test for that function is no longer useful
static void check_n_list_elements_ok(list_admin* admin, int nr_items, const test_struct* verify_data)
{
    for(int i = 0; i < nr_items; i++)
    {
        test_struct* ptr = list_get_element(admin, i);
        TEST_ASSERT_NOT_NULL(ptr);
        TEST_ASSERT_EQUAL(0, memcmp(verify_data+i, ptr, sizeof(*ptr)));
    }
}


void setUp(void)
{
    // This is run before EACH test
    mylist = construct_list(sizeof(test_struct));
    TEST_ASSERT_NOT_NULL(mylist);
}

void tearDown(void)
{
    // This is run after EACH test
    destruct_list(&mylist);
    TEST_ASSERT_NULL(mylist);
}

static void test_ConstructList(void)
{
    TEST_ASSERT_NULL(mylist->head);
    TEST_ASSERT_EQUAL(sizeof(test_struct), mylist->element_size);
    TEST_ASSERT_EQUAL(0, mylist->nr_elements);
}

static void test_NrElements_parameters(void)
{
    TEST_ASSERT_EQUAL(-1, list_get_nr_elements(NULL));
}

static void test_AddData_parameters(void)
{
    TEST_ASSERT_EQUAL(-1, list_add_tail(mylist, NULL));
    TEST_ASSERT_EQUAL(-1, list_add_tail(NULL, mylist));
}

static void test_AddData(void)
{
    test_struct data = {3, 4};
    TEST_ASSERT_EQUAL(0, list_add_tail(mylist, &data));
    
    TEST_ASSERT_NOT_NULL(mylist->head);
    TEST_ASSERT_EQUAL(1, list_get_nr_elements(mylist));
    list_head* head = mylist->head;
    test_struct* element = (test_struct*)(head+1);
    TEST_ASSERT_EQUAL(data.i, element->i);
    TEST_ASSERT_EQUAL(data.j, element->j);
}

static void test_Add2PiecesOfData(void)
{
    test_struct data1 = {8, 9};
    test_struct data2 = {-3, -4};
    TEST_ASSERT_EQUAL(0, list_add_tail(mylist, &data1));
    TEST_ASSERT_EQUAL(0, list_add_tail(mylist, &data2));

    TEST_ASSERT_NOT_NULL(mylist->head);
    TEST_ASSERT_NOT_NULL(mylist->head->next);
    TEST_ASSERT_EQUAL(2, list_get_nr_elements(mylist));
    list_head* head = mylist->head->next;
    test_struct* element = (test_struct*)(head+1);
    TEST_ASSERT_EQUAL(data2.i, element->i);
    TEST_ASSERT_EQUAL(data2.j, element->j);
}

static void test_GetElement_parameters(void)
{
    TEST_ASSERT_NULL(list_get_element(NULL, 0));
}

static void test_GetElement(void)
{
    const test_struct data[] = {{8, 9}, {-3, -4}, {21, 56}, {0, 0}};
    const int nr_elements = sizeof(data)/sizeof(data[0]);

    for(int i = 0; i < nr_elements; i++)
    {
        TEST_ASSERT_EQUAL(0, list_add_tail(mylist, &(data[i])));
    }

    check_n_list_elements_ok(mylist, nr_elements, data); // checks using list_get_element

    TEST_ASSERT_NULL(list_get_element(mylist, nr_elements));
    TEST_ASSERT_NULL(list_get_element(mylist, -1));
}

static void test_GetLast_parameters(void)
{
    TEST_ASSERT_NULL(list_get_last(NULL));
}

static void test_GetLast(void)
{
    const test_struct data[] = {{8, 9}, {-3, -4}, {21, 56}, {0, 0}};
    const int nr_elements = sizeof(data)/sizeof(data[0]);

    TEST_ASSERT_NULL(list_get_last(mylist));
    for(int i = 0; i < nr_elements; i++)
    {
        TEST_ASSERT_EQUAL(0, list_add_tail(mylist, &(data[i])));
        test_struct* ptr = list_get_last(mylist);
        TEST_ASSERT_NOT_NULL(ptr);
        TEST_ASSERT_EQUAL(0, memcmp(&(data[i]), ptr, sizeof(*ptr)));
    }
}

static void test_IndexOf_parameters(void)
{
    TEST_ASSERT_EQUAL(-1, list_index_of(mylist, NULL));
    TEST_ASSERT_EQUAL(-1, list_index_of(NULL, mylist));
}

static void test_IndexOf(void)
{
    const test_struct real_data[] = {{8, 9}, {-3, -4}, {21, 56}};
    const int nr_real_elements = sizeof(real_data)/sizeof(real_data[0]);
    const test_struct false_data[] = {{-3, 9}, {8, 56}, {21, -4}, {0, 0}};
    const int nr_false_elements = sizeof(false_data)/sizeof(false_data[0]);
    for(int i = 0; i < nr_real_elements; i++)
    {
        TEST_ASSERT_EQUAL(0, list_add_tail(mylist, &(real_data[i])));
    }

    for(int i = 0; i < nr_real_elements; i++)
    {
        TEST_ASSERT_EQUAL(i, list_index_of(mylist, &(real_data[i])));
    }
    for(int i = 0; i < nr_false_elements; i++)
    {
        TEST_ASSERT_EQUAL(-1, list_index_of(mylist, &(false_data[i])));
    }
}

static void test_IndexOfSpecial_parameters(void)
{
    TEST_ASSERT_EQUAL(-1, list_index_of_special(mylist, NULL, compare_test_func));
    TEST_ASSERT_EQUAL(-1, list_index_of_special(NULL, mylist, compare_test_func));
    TEST_ASSERT_EQUAL(-1, list_index_of_special(mylist, mylist, NULL));
}

static void test_IndexOfSpecial(void)
{
    const test_struct data[] = {{8, 9}, {-3, -4}, {21, 56}}; // data in list
    const test_struct real_data[] = {{8, -4}, {-3, 56}, {21, 9}}; // data to search for (i equals)
    const int nr_real_elements = sizeof(real_data)/sizeof(real_data[0]);
    const test_struct false_data[] = {{-2, 9}, {9, -4}, {22, 56}, {0, 0}}; // data that doesn't exist
    const int nr_false_elements = sizeof(false_data)/sizeof(false_data[0]);

    // first make sure our compare function works
    for(int i = 0; i < nr_real_elements; i++)
    {
        TEST_ASSERT_EQUAL(0, compare_test_func(&(data[i]), &(real_data[i]), 0));
        for (int j = 0; j < nr_false_elements; j++)
        {
            TEST_ASSERT_NOT_EQUAL(0, compare_test_func(&(data[i]), &(false_data[j]), 0))
        }
    }

    for(int i = 0; i < nr_real_elements; i++)
    {
        TEST_ASSERT_EQUAL(0, list_add_tail(mylist, &(data[i])));
    }

    for(int i = 0; i < nr_real_elements; i++)
    {
        TEST_ASSERT_EQUAL(i, list_index_of_special(mylist, &(real_data[i]), compare_test_func));
    }
    for(int i = 0; i < nr_false_elements; i++)
    {
        TEST_ASSERT_EQUAL(-1, list_index_of_special(mylist, &(false_data[i]), compare_test_func));
    }
}

static void test_DeleteItem_parameters(void)
{
    TEST_ASSERT_EQUAL(-1, list_delete_item(NULL, 0));
    TEST_ASSERT_EQUAL(-1, list_delete_item(mylist, -1));
}

static void test_DeleteItem(void)
{
    const test_struct data[] = {{8, 9}, {-3, -4}, {21, 56}, {0, 0}, {12, 12345}};
    int nr_elements = sizeof(data)/sizeof(data[0]);
    const test_struct data_noFirst[] = {{-3, -4}, {21, 56}, {0, 0}, {12, 12345}};
    const test_struct data_noLast[] = {{-3, -4}, {21, 56}, {0, 0}};
    const test_struct data_noMiddle[] = {{-3, -4}, {0, 0}};

    TEST_ASSERT_EQUAL(-1, list_delete_item(mylist, 0));
    for(int i = 0; i < nr_elements; i++)
    {
        TEST_ASSERT_EQUAL(0, list_add_tail(mylist, &(data[i])));
    }

    TEST_ASSERT_EQUAL(0, list_delete_item(mylist, 0));
    nr_elements--;
    TEST_ASSERT_EQUAL(nr_elements, list_get_nr_elements(mylist));

    check_n_list_elements_ok(mylist, nr_elements, data_noFirst);

    TEST_ASSERT_EQUAL(0, list_delete_item(mylist, nr_elements-1));
    nr_elements--;
    TEST_ASSERT_EQUAL(nr_elements, list_get_nr_elements(mylist));
    
    check_n_list_elements_ok(mylist, nr_elements, data_noLast);

    TEST_ASSERT_EQUAL(0, list_delete_item(mylist, 1));
    nr_elements--;
    TEST_ASSERT_EQUAL(nr_elements, list_get_nr_elements(mylist));
    
    check_n_list_elements_ok(mylist, nr_elements, data_noMiddle);
}

static void test_DeleteLast_parameters(void)
{
    TEST_ASSERT_EQUAL(-1, list_delete_last(NULL));
}

static void test_DeleteLast(void)
{
    const test_struct data[] = {{8, 9}, {-3, -4}, {21, 56}, {0, 0}, {12, 12345}};
    int nr_elements = sizeof(data)/sizeof(data[0]);

    TEST_ASSERT_EQUAL(-1, list_delete_last(mylist));
    for(int i = 0; i < nr_elements; i++)
    {
        TEST_ASSERT_EQUAL(0, list_add_tail(mylist, &(data[i])));
    }

    for (int i = nr_elements-1; i >= 0; i--)
    {
        TEST_ASSERT_EQUAL(0, list_delete_last(mylist));
        TEST_ASSERT_EQUAL(i, list_get_nr_elements(mylist));
        check_n_list_elements_ok(mylist, i, data);
    }

    TEST_ASSERT_NULL(mylist->head);
    TEST_ASSERT_EQUAL(0, list_get_nr_elements(mylist));
}

int main(void)
{
    UnityBegin();
    
    MY_RUN_TEST(test_ConstructList);
    MY_RUN_TEST(test_NrElements_parameters);
    MY_RUN_TEST(test_AddData_parameters);
    MY_RUN_TEST(test_AddData);
    MY_RUN_TEST(test_Add2PiecesOfData);
    MY_RUN_TEST(test_GetElement_parameters);
    MY_RUN_TEST(test_GetElement);
    MY_RUN_TEST(test_GetLast_parameters);
    MY_RUN_TEST(test_GetLast);
    MY_RUN_TEST(test_IndexOf_parameters);
    MY_RUN_TEST(test_IndexOf);
    MY_RUN_TEST(test_IndexOfSpecial_parameters);
    MY_RUN_TEST(test_IndexOfSpecial);
    MY_RUN_TEST(test_DeleteItem_parameters);
    MY_RUN_TEST(test_DeleteItem);
    MY_RUN_TEST(test_DeleteLast_parameters);
    MY_RUN_TEST(test_DeleteLast);
    // MY_RUN_TEST();
    // MY_RUN_TEST();
    // MY_RUN_TEST();
    // MY_RUN_TEST();
    
    return UnityEnd();
}
