#include <stdio.h>
#include "ex22.h"
#include "dbg.h"

/* 
 * static FILE is the inverse of extern
 * says that the variable is only used in 
 * this c file and shouldn't be available
 * in other parts of the program
 */
struct State state = {.the_age = 31, .the_size = 1000};

int get_age()
{
    // return THE_AGE;
    return state.the_age;
}

void set_age(int age)
{
    // THE_AGE = age;
   state.the_age = age; 
}

double update_ratio(double new_ratio)
{
    /* 
     * static FUNCTION. if you declare 
     * a variable in a function static,
     * then the variable acts like a static
     * defined in the file, but it's only 
     * accessible from that function.
     * it's a way of creating constant state
     * for a function, but in reality it's 
     * rarely used in modern C programming
     * because it's hard to use in threads
     */
    static double ratio = 1.0;

    double old_ratio = ratio;
    ratio = new_ratio;

    return old_ratio;
}

void print_size()
{
    log_info("I think size is: %d", state.the_size);
}
