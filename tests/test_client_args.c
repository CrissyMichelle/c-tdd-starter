#include "unity.h"
#include "../src/client.h"

void setUp(void) {}
void tearDown(void) {}

void test_parse_endoint_requires_hostname_and_port(void)
{
    endpoint_t ep;

    char *argv1[] = {"client", "example.com", NULL};
    TEST_ASSERT_NOT_EQUAL(0, parse_endpoint(2, argv1, &ep)); // missing port
    
    char *argv2[] = {"client", NULL, "8080"};
    TEST_ASSERT_NOT_EQUAL(0, parse_endpoint(3, argv2, &ep)); // missing hostname

    char *argv3[] = {"client", "example.com", "8080"};
    TEST_ASSERT_EQUAL(0, parse_endpoint(3, argv3, &ep));
    TEST_ASSERT_EQUAL_STRING("example.com", ep.hostname);
    TEST_ASSERT_EQUAL_STRING("8080", ep.port);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_parse_endoint_requires_hostname_and_port);
    return UNITY_END();
}
