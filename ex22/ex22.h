#ifndef _ex22_h
#define _ex22_h

/* extern tells the compiler that the variable exists
 * but it's in a different location
 */
// extern int THE_SIZE;

struct State {
    int the_size;
    int the_age;
};
extern struct State state;

// gets and sets an internal static variable in ex22.c
int get_age();
void set_age(int age);

// updates a static variable that's inside update_ratio
double update_ratio(double ratio);

void print_size();

#endif
