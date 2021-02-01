/*
 * variable argument functions
 * * this enables better interfaces for your library
 * * this allows you to create your own printf
 * -> Variable length argument is a feature that allows a function to
 * -> receive any number of arguments. There are situations where we want
 * -> a function to handle variable number of arguments according to requirement.
 * -> 1) Sum of given numbers.
 * -> 2) Minimum of given numbers.
 */


/*
 * read_scan replaces fgets here
 * read_scan(const char *fmt, ...)
 */

/* malloc vs calloc differences
 * malloc() allocates memory block of given 
 * size (in bytes) and returns a pointer to 
 * the beginning of the block. malloc() doesn’t
 * initialize the allocated memory. If we try to
 * access the content of memory block(before 
 * initializing) then we’ll get segmentation 
 * fault error(or maybe garbage values).
 * 
 * calloc() allocates the memory and also 
 * initializes the allocated memory block to 
 * zero. If we try to access the content of 
 * these blocks then we’ll get 0.
 */

/* what rc means: return code. either of 0/NULL 
 * or 1 form.
 */

/* 
 * va_list, va_start, va_arg, va_end
 */

/*
 * an important pattern (mutating)
 * init pointer to NULL
 * like int *i = NULL;
 * then pass the address to a fcn
 * ... that mallocs/callocs it
 * ... then uses a fcn to write values in it
 * ... then checks if it's working
*/

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include "dbg.h"

#define MAX_DATA 100


// it grabs the pointer of the out_string bytearray
// then using fgets, it writes on the out_string
// as fgets returns a str on success, null on fail,
// do the check accordingly
int read_string(char **out_string, int max_buffer)
{
    // calloc: https://en.cppreference.com/w/c/memory/calloc

    /* Allocates memory for an array of num objects
     * of size and initializes all bytes in the 
     * allocated storage to zero. 
     */
    *out_string = calloc(1, max_buffer + 1);
    check_mem(*out_string);

    char *result = fgets(*out_string, max_buffer, stdin);
    check(result != NULL, "Input error.");

    return 0;

error:
    if (*out_string) free(*out_string);
    *out_string = NULL;
    return -1;
}

int read_int(int *out_int)
{
    char *input = NULL;
    int rc = read_string(&input, MAX_DATA);
    check(rc == 0, "Failed to read number.");

    *out_int = atoi(input);

    free(input);
    return 0;

error:
    if (input) free(input);
    return -1;
}

int read_scan(const char *fmt, ...)
{
    int i = 0;
    int rc = 0;
    // pointer of int is null
    int *out_int = NULL;
    // pointer of char is null
    char *out_char = NULL;
    // pointer of bytearray is null
    char **out_string = NULL;
    int max_buffer = 0;

    // va_list is an object type for holding
    // macros: https://en.cppreference.com/w/c/variadic/va_list
    va_list argp;

    // va_start enables access to variable
    // arguments via fmt, and requires
    // va_start should be invoked with an 
    // instance to a valid va_list object argp
    // before any calls to va_arg.
    // https://en.cppreference.com/w/c/variadic/va_start

    va_start(argp, fmt);


    // va_arg looks at the next parameter 
    // in va_list ap
    // The va_arg macro expands to an expression of type T that corresponds to the next parameter from the va_list ap.
    // Now, to get a variable from the va_list argp, I use the macro va_arg(argp, TYPE) where TYPE is the exact type of what I will assign this function parameter to. The downside to this design is that you’re flying blind, so if you don’t have enough parameters, then oh well, you’ll most likely crash.
    // https://en.cppreference.com/w/c/variadic/va_arg

    for (i = 0; fmt[i] != '\0'; i++) {
        debug("fmt[i]: %c", fmt[i]);
        // when i hit a format (%)
        // use the switch statement
        // to figure out what to do
        if (fmt[i] == '%') {
            // increment to get the next letter
            // in the format
            i++;
            switch (fmt[i]) {
                case '\0':
                    sentinel("Invalid format, you ended with %%.");
                    break;

                // digit format
                // next argument here is age
                case 'd':
                    out_int = va_arg(argp, int *);
                    rc = read_int(out_int);
                    check(rc == 0, "Failed to read int.");
                    break;

                // char format
                // next argument here is char
                case 'c':
                    out_char = va_arg(argp, char *);
                    *out_char = fgetc(stdin);
                    break;

                // string format
                case 's':
                    max_buffer = va_arg(argp, int);
                    out_string = va_arg(argp, char **);
                    rc = read_string(out_string, max_buffer);
                    check(rc == 0, "Failed to read string.");
                    break;

                default:
                    sentinel("Invalid format.");
            }
        } else {
            fgetc(stdin);
        }

        check(!feof(stdin) && !ferror(stdin), "Input error.");
    }

    // https://en.cppreference.com/w/c/variadic/va_end
    va_end(argp);
    return 0;

error:
    va_end(argp);
    return -1;
}

int main(int argc, char *argv[])
{
    char *first_name = NULL;
    char initial = ' ';
    char *last_name = NULL;
    int age = 0;

    printf("What's your first name? ");
    int rc = read_scan("%s", MAX_DATA, &first_name);
    check(rc == 0, "Failed first name.");

    printf("What's your initial? ");
    rc = read_scan("%c\n", &initial);
    check(rc == 0, "Failed initial.");

    printf("What's your last name? ");
    rc = read_scan("%s", MAX_DATA, &last_name);
    check(rc == 0, "Failed last name.");

    printf("How old are you? ");
    rc = read_scan("%d", &age);

    printf("---- RESULTS ----\n");
    printf("First Name: %s", first_name);
    printf("Initial: '%c'\n", initial);
    printf("Last Name: %s", last_name);
    printf("Age: %d\n", age);

    free(first_name);
    free(last_name);
    return 0;
error:
    return -1;
}
