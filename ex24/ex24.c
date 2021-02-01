/*
 * fgets, fscanf
 * - use fgets for grabbing char inputs
 * - use scanf for grabbing digit inputs
 * - but note that scanf is crap
 * fscanf has issues because you can't tell fscanf
 * how big of a buffer is before you read it
 */
#include <stdio.h>
#include "dbg.h"

#define MAX_DATA 100

/* what an enum allows is that 
 * it allows you to define names on 
 * integral constants
 * by default, it's 1, 2, 3, 4, etc.
 */
typedef enum EyeColor {
    BLUE_EYES, GREEN_EYES, BROWN_EYES,
    BLACK_EYES, OTHER_EYES
} EyeColor;

// this is a byte array of eye_color_names
// capitalized to indicate that it's a constant
const char *EYE_COLOR_NAMES[] = {
    "Blue", "Green", "Brown", "Black", "Other"
};

typedef struct Person {
    int age;
    char first_name[MAX_DATA];
    char last_name[MAX_DATA];
    EyeColor eyes;
    float income;
} Person;

// 
// fscanf
// int fscanf( FILE *restrict stream, const char *restrict format, ... );
// - stream: input file stream to read from
// - buffer: pointer to a null-terminated character string to read from
// - format: pointer to a null-terminated character string specifying how to read the input.

int main(int argc, char *argv[])
{
    Person you = {.age = 0 };
    int i = 0;

    // buffer for your inputs
    char *in = NULL;

    // fgets = stores count characters within the first argument
    // stdin = where to read data from
    printf("What's your First Name? ");
    in = fgets(you.first_name, MAX_DATA - 1, stdin);
    check(in != NULL, "Failed to read first name.");

    printf("What's your Last Name? ");
    in = fgets(you.last_name, MAX_DATA - 1, stdin);
    check(in != NULL, "Failed to read last name.");

    // Reads data from the a variety of sources, interprets it according to format and stores the results into given locations.
    // you have to give the address of you.age so that fscanf has a pointer
    // to work with
    printf("How old are you? ");
    int rc = fscanf(stdin, "%d", &you.age);
    check(rc > 0, "You have to enter a number.");

    printf("What color are your eyes:\n");
    for (i = 0; i <= OTHER_EYES; i++) {
        printf("%d) %s\n", i + 1, EYE_COLOR_NAMES[i]);
    }
    printf("> ");

    int eyes = -1;
    rc = fscanf(stdin, "%d", &eyes);
    check(rc > 0, "You have to enter a number.");

    you.eyes = eyes - 1;
    check(you.eyes <= OTHER_EYES
            && you.eyes >= 0, "Do it right, that's not an option.");

    printf("How much do you make an hour? ");
    rc = fscanf(stdin, "%f", &you.income);
    check(rc > 0, "Enter a floating point number.");

    printf("----- RESULTS -----\n");

    printf("First Name: %s", you.first_name);
    printf("Last Name: %s", you.last_name);
    printf("Age: %d\n", you.age);
    printf("Eyes: %s\n", EYE_COLOR_NAMES[you.eyes]);
    printf("Income: %f\n", you.income);

    return 0;
error:

    return -1;
}
