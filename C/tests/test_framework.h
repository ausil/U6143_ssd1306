/*
 * Simple test framework for C code
 */
#ifndef TEST_FRAMEWORK_H
#define TEST_FRAMEWORK_H

#include <stdio.h>
#include <string.h>

/* Test statistics */
static int tests_run = 0;
static int tests_passed = 0;
static int tests_failed = 0;

/* Color codes for terminal output */
#define COLOR_GREEN "\033[0;32m"
#define COLOR_RED   "\033[0;31m"
#define COLOR_RESET "\033[0m"

/* Assert macros */
#define ASSERT_TRUE(condition, message) do { \
    tests_run++; \
    if (condition) { \
        tests_passed++; \
        printf(COLOR_GREEN "  ✓ " COLOR_RESET "%s\n", message); \
    } else { \
        tests_failed++; \
        printf(COLOR_RED "  ✗ " COLOR_RESET "%s\n", message); \
        printf("    Failed at %s:%d\n", __FILE__, __LINE__); \
    } \
} while(0)

#define ASSERT_FALSE(condition, message) \
    ASSERT_TRUE(!(condition), message)

#define ASSERT_EQUAL(expected, actual, message) \
    ASSERT_TRUE((expected) == (actual), message)

#define ASSERT_STR_EQUAL(expected, actual, message) \
    ASSERT_TRUE(strcmp((expected), (actual)) == 0, message)

/* Test suite macros */
#define TEST_SUITE(name) \
    printf("\n" COLOR_GREEN "Running test suite: " COLOR_RESET "%s\n", name)

#define TEST_RESULTS() do { \
    printf("\n" COLOR_GREEN "==========================================\n"); \
    printf("Test Results:\n"); \
    printf("  Total:  %d\n", tests_run); \
    printf("  Passed: %d\n", tests_passed); \
    printf("  Failed: %d\n", tests_failed); \
    if (tests_failed == 0) { \
        printf("\n✓ All tests passed!\n"); \
    } else { \
        printf("\n✗ Some tests failed!\n"); \
    } \
    printf("==========================================" COLOR_RESET "\n\n"); \
    return tests_failed; \
} while(0)

#endif /* TEST_FRAMEWORK_H */
