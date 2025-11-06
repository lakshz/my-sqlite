#include "unity.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*
 * Helper to run the compiled DB program with a sequence of commands
 */
static char *run_script(const char *commands)
{
    system("rm -f output.txt");
    FILE *f = fopen("script.txt", "w");
    fprintf(f, "%s", commands);
    fclose(f);

    // Run db program, redirecting stdin from script.txt and stdout to output.txt
    system("./db < script.txt > output.txt");

    // Read full output
    FILE *out = fopen("output.txt", "r");
    fseek(out, 0, SEEK_END);
    long size = ftell(out);
    fseek(out, 0, SEEK_SET);
    char *buffer = malloc(size + 1);
    fread(buffer, 1, size, out);
    buffer[size] = '\0';
    fclose(out);
    return buffer;
}

/* Required by Unity (can be empty if not using fixtures) */
void setUp(void) {}
void tearDown(void) {}

void test_insert_and_select(void)
{
    const char *commands =
        "insert 1 user1 person1@example.com\n"
        "select\n"
        ".exit\n";

    char *output = run_script(commands);

    TEST_ASSERT_NOT_NULL(strstr(output, "1 user1 person1@example.com"));
    TEST_ASSERT_NOT_NULL(strstr(output, "Executed."));

    free(output);
}

void test_table_full_error(void)
{
    // Insert many rows to trigger "Table full"
    FILE *f = fopen("script.txt", "w");
    for (int i = 0; i < 1500; i++)
    {
        fprintf(f, "insert %d user%d person%d@example.com\n", i, i, i);
    }
    fprintf(f, ".exit\n");
    fclose(f);

    system("./db < script.txt > output.txt");
    FILE *out = fopen("output.txt", "r");
    fseek(out, 0, SEEK_END);
    long size = ftell(out);
    fseek(out, 0, SEEK_SET);
    char *buffer = malloc(size + 1);
    fread(buffer, 1, size, out);
    buffer[size] = '\0';
    fclose(out);

    TEST_ASSERT_NOT_NULL(strstr(buffer, "Error: Table full."));

    free(buffer);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_insert_and_select);
    RUN_TEST(test_table_full_error);
    return UNITY_END();
}
