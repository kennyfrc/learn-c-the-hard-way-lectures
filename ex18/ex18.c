/**
 * Some key Ideas
 * 1. function pointer (non-callbackable): 
 * `int func(a,b) -> int (*func)(a,b) -> int (*new_name)(a,b)`
 * 1.a. you can use this to cast them into functions
 * 1.b. inuition is that you PASS a defined function to a function pointer that has been undefined but with allocated memory
 * 
 * 2. function pointer (callbackable): the type is now a new_name 
 * `typedef int (*new_name)(a,b)`
 * 3. memcpy (no care nul) vs strcpy
**/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

/** Our old friend die from ex17. */
void die(const char *message)
{
    if (errno) {
        perror(message);
    } else {
        printf("ERROR: %s\n", message);
    }

    exit(1);
}

/* this makes compare_cb a function pointer that 
 * you can pass as a argument
*/
typedef int (*compare_cb) (int a, int b);

/**
 * A classic bubble sort function that uses the 
 * compare_cb to do the sorting. 
 * variable name of the pointer is compare_cb
 * and you use it like a function
 */
int *bubble_sort(int *numbers, int count, compare_cb cmp)
{
    int temp = 0;
    int i = 0;
    int j = 0;

    /* because this target will "work hard", it's 
     * best to put this at heap memory
    **/
    int *target = malloc(count * sizeof(int));

    if (!target)
        die("Memory error.");

    /* the objective of memcpy is to copy a string
     * while ignoring the /0 nul character
     * in contrast, strcpy doesn't care
     */
    memcpy(target, numbers, count * sizeof(int));

    /* 
     * this is the classic bubble sort
     * - one pass will bubble up the biggest 
     * element to the end
     * - therefore, if an array is size n, then
     * you want to bubble it up n times
     */
    for (i = 0; i < count; i++) {
        for (j = 0; j < count - 1; j++) {
            if (cmp(target[j], target[j + 1]) > 0) {
                temp = target[j + 1];
                target[j + 1] = target[j];
                target[j] = temp;
            }
        }
    }

    return target;
}

int sorted_order(int a, int b)
{
    return a - b;
}

int reverse_order(int a, int b)
{
    return b - a;
}

int strange_order(int a, int b)
{
    if (a == 0 || b == 0) {
        return 0;
    } else {
        return a % b;
    }
}

/** 
 * Used to test that we are sorting things correctly
 * by doing the sort and printing it out.
 */
void test_sorting(int *numbers, int count, compare_cb cmp)
{
    int i = 0;
    /* bubble sort allocates memory 
     * - the best clue is the pointer
     */
    int *sorted = bubble_sort(numbers, count, cmp);

    // check as usual
    if (!sorted)
        die("Failed to sort as requested.");

    // print the numbers
    for (i = 0; i < count; i++) {
        printf("%d ", sorted[i]);
    }

    // end line
    printf("\n");

    // free the memory
    free(sorted);
}

void destroy(compare_cb cmp)
{
    int i = 0;

    unsigned char *data = (unsigned char *)cmp;

    for(i = 0; i < 1; i++) {
        data[i] = i;
    }

    printf("\n");
}

/* converts function to a string
 * then it prints out its context in hex code 
 * this shows the raw assembler bytecode
 */ 
void dump(compare_cb cmp)
{
    int i = 0;

    unsigned char *data = (unsigned char *)cmp;

    for(i = 0; i < 25; i++) {
        printf("%02x:", data[i]);
    }

    printf("\n");
}



int main(int argc, char *argv[])
{
    if (argc < 2) die("USAGE: ex18 4 3 1 5 6");

    // number of actual arguments
    int count = argc - 1;
    int i = 0;
    // pointer to a pointer + 1
    // this essentially has moved it to the first argument
    char **inputs = argv + 1;

    // initialize an array using the heap
    int *numbers = malloc(count * sizeof(int));
    if (!numbers) die("Memory error.");

    // assign the arguments to the array
    for (i = 0; i < count; i++) {
        numbers[i] = atoi(inputs[i]);
    }

    // pass the sorted_order, reverse_order, and strange_order functions
    test_sorting(numbers, count, sorted_order);
    test_sorting(numbers, count, reverse_order);
    test_sorting(numbers, count, strange_order);

    printf("SORTED: ");
    dump(sorted_order);

    printf("REVERSE: ");
    dump(reverse_order);

    free(numbers);

    return 0;
}
