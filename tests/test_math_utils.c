#include "unity.h"
#include "../src/math_utils.h"

void setUp(void) {}
void tearDown(void) {}

void test_add_two_positive_numbers(void)
{
    TEST_ASSERT_EQUAL_INT(5, add(2, 3));
}

void test_add_negative_numbers(void)
{
    TEST_ASSERT_EQUAL_INT(-8, add(-3, -5));
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_add_two_positive_numbers);
    RUN_TEST(test_add_negative_numbers);
    return UNITY_END();
}
