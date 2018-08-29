#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void sig_handle(int sig)
{
    printf("\nInterrupt. Bye.");
    fflush(stdout);
    exit(EXIT_SUCCESS);
}

int is_numeric(char* arg)
{
    for (int i = 0; i < strlen(arg); i++) {
        if (arg[i] == '\n') {
            continue;
        }
        if (arg[i] < '0' || arg[i] > '9') {
            return 0;
        }
    }
    return 1;
}

void printBitMap(char* buf, int sz, int curr_pos)
{
    for (int i = 0; i < sz; i++) {

        if (i != 0 && i % 4 == 0) {
            printf("\t=======\n");
        }
        if (curr_pos == i / sizeof(int) && i % sizeof(int) == 0) {
            printf("-->");
        }
        printf("\t");
        for (int j = 7; j >= 0; j--) {
            printf("%d", (buf[i] & (1 << j)) > 0);
        }
        printf("\n");
    }
}

int main(int argc, char** argv)
{
    signal(SIGINT, &sig_handle);
    printf("Reverse Polish Notation Interactive\n");
    printf("Enter stack size (number of ints): ");
    char stack_sz[10];
    fgets(stack_sz, 10, stdin);
    int sz = atoi(stack_sz);
    int stack[sz];
    memset(stack, 0, sz * sizeof(int));

    char input[10];
    int curr_pos = 0;
    int* pos;
    int first_arg, second_arg, results, has_args = 0;
    printf("Enter a integer (-2147483648 to 2147483647) to add to the stack or a mathematical operator (+,-,*,/) to perform operations on the stack:\n");
    printf("(press 'q' + [ENTER] to quit)");

    while (1) {

        fgets(input, 10, stdin);
        if (curr_pos >= 2) {
            has_args = 1;
        } else {
            has_args = 0;
        }

        if (strcmp(input, "+\n") == 0) {
            if (!has_args) {
                printf("error: not enough args. \n");
                continue;
            }
            stack[curr_pos - 2] = stack[curr_pos - 2] + stack[curr_pos - 1];
            curr_pos = (curr_pos - 1) % sz;
        } else if (strcmp(input, "-\n") == 0) {
            if (!has_args) {
                printf("error: not enough args. \n");
                continue;
            }
            stack[curr_pos - 2] = stack[curr_pos - 2] - stack[curr_pos - 1];
            curr_pos = (curr_pos - 1) % sz;
        } else if (strcmp(input, "*\n") == 0) {
            if (!has_args) {
                printf("error: not enough args. \n");
                continue;
            }
            stack[curr_pos - 2] = stack[curr_pos - 2] * stack[curr_pos - 1];
            curr_pos = (curr_pos - 1) % sz;
        } else if (strcmp(input, "/\n") == 0) {
            if (!has_args) {
                printf("error: not enough args. \n");
                continue;
            }
            if (stack[curr_pos - 1] == 0) {

                printf("error: Cannot divide by 0. Remove from stack ? [y/n]");
                char res[10];
                fgets(res, 10, stdin);
                if (strcmp(res, "y\n") == 0) {
                    printf("Zero at position %d removed. Enter a new integer or operator\n", curr_pos - 1);
                    curr_pos = (curr_pos - 1) % sz;
                    continue;
                }
            }
            stack[curr_pos - 2] = stack[curr_pos - 2] / stack[curr_pos - 1];
            curr_pos = (curr_pos - 1) % sz;
        } else if (is_numeric(input)) {
            stack[curr_pos] = atoi(input);
            curr_pos = (curr_pos + 1) % sz;
        } else if (strcmp(input, "q\n") == 0) {
            printf("Bye.");
            exit(EXIT_SUCCESS);
        } else {
            printf("error: Invalid input. \n");
            printf("Enter a integer (-2147483648 to 2147483647) to add to the stack or a mathematical operator (+,-,*,/) to perform operations on the stack:\n");
            continue;
        }

        printf("\tStack Memory Layout\n\t----------------\n");
        printBitMap((char*)stack, sz * sizeof(int), curr_pos);

        printf("\t Stack Decimal Equivalent\n\t--------------\n");
        for (int i = 0; i < curr_pos; i++) {
            printf("\t%d\n", stack[i]);
        }
    }
}