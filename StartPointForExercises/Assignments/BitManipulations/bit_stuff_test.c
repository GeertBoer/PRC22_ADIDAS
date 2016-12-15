#include "bit_stuff.h"
#include "unity.h"

// I rather dislike keeping line numbers updated, so I made my own macro to ditch the line number
#define MY_RUN_TEST(func) RUN_TEST(func, 0)


void setUp(void)
{
    // This is run before EACH test
}

void tearDown(void)
{
    // This is run after EACH test
}


static void test_bitMaskNrBits(void)
{
    TEST_ASSERT_EQUAL_HEX32(0x1, makeBitmask(1, 0));
    TEST_ASSERT_EQUAL_HEX32(0x3, makeBitmask(2, 0));
    TEST_ASSERT_EQUAL_HEX32(0x1f, makeBitmask(5, 0));
    TEST_ASSERT_EQUAL_HEX32(0xffffffff, makeBitmask(32, 0));
}

static void test_bitMaskShift(void)
{
    TEST_ASSERT_EQUAL_HEX32(0x2, makeBitmask(1, 1));
    TEST_ASSERT_EQUAL_HEX32(0x80, makeBitmask(1, 7));
    TEST_ASSERT_EQUAL_HEX32(0x100, makeBitmask(1, 8));
    TEST_ASSERT_EQUAL_HEX32(0x38000, makeBitmask(3, 15));
    TEST_ASSERT_EQUAL_HEX32(0xC0000000, makeBitmask(8, 30));
}

static void test_countOnes(void)
{
    TEST_ASSERT_EQUAL_UINT32(0, countOnes(0x0));
    TEST_ASSERT_EQUAL_UINT32(32, countOnes(0xffffffff));
    TEST_ASSERT_EQUAL_UINT32(16, countOnes(0x5a5a5a5a));
}

static void test_countOnesInBitMask(void)
{
    unsigned int i = 0;
    for (i = 0; i < 32; i++)
    {
        TEST_ASSERT_EQUAL_UINT32(i, countOnes(makeBitmask(i, 0)));
    }
}

int main(void)
{
    UnityBegin();
    
    MY_RUN_TEST(test_bitMaskNrBits);
    MY_RUN_TEST(test_bitMaskShift);
    MY_RUN_TEST(test_countOnes);
    MY_RUN_TEST(test_countOnesInBitMask);

    return UnityEnd();
}
