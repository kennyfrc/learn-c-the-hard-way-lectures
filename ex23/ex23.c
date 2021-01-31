/*
 * Duff's device
 * manual implementation of loop unrolling
 * by interleaving switch and do/while
 * 
 * this device copies content from one array
 * to another via *to++ = *from++
 * 
 * Problem
 * Duff wanted use it to speed up a real-time 
 * animation program
 * 
 * Today: it's not needed
 * 
 * Idea
 * the number of instructions executed in a
 * loop can be reduced by reducing the number of 
 * loop tests, sometimes reducing the amount of 
 * time spent in the loop
 * 
 * Duff's device
 * Duff's device provides a solution by 
 * first performing the remainder of iterations,
 * followed by iterating as many times as 
 * necessary the multiple of eight similar 
 * instructions. 
 * 
 * 1. Get the remainder of iterations
 * 2. Throw it into a switch statement
 * 3. Use the do/while loop to unroll as
 *    necessary (together with the switch)
 * 4. This reduces the time required to unroll
 */

#include <stdio.h>
#include <string.h>
#include "dbg.h"

/* 
 * this is a normal approach to filling 
 * this is a plain for loop
 */
int normal_copy(char *from, char *to, int count)
{
    int i = 0;

    for (i = 0; i < count; i++) {
        to[i] = from[i];
    }

    return i;
}

int duffs_device(char *from, char *to, int count)
{
    {
         /* this results to 13:
          * by default, this rounds down
          * first performing the remainder of iterations, followed by iterating as many times as necessary the multiple of eight similar instructions. 
          */ 
        // 100 + 7 / 8
        int n = (count + 7) / 8;

        /* 
         * if it's divisible by 8, 
         * then increment the pointer by
         * the size of its type
         

        /*
         * The trick is to use a switch statement 
         * where all but one of the cases labels 
         * are in the middle of a while loop. 
         * Further, all cases fall through to the 
         * end of the while loop. Despite the 
         * impression, it makes Duff’s device is 
         * legal C and C++ code
        */

        /* 
         * Loop unrolling, also known as loop
         * unwinding, is a loop transformation
         * technique that attempts to optimize a 
         * program's execution speed at the 
         * expense of its binary size, which is 
         * an approach known as space–time 
         * tradeoff.
         */
        switch (count % 8) {
            case 0:
                do {
                    *to++ = *from++;        
                    case 7:
                    *to++ = *from++;
                    // log_info("from: %s", from);
                    // log_info("to  : %s", to);
                    case 6:
                    *to++ = *from++;
                    // log_info("from: %s", from);
                    // log_info("to  : %s", to);
                    case 5:
                    *to++ = *from++;
                    // log_info("from: %s", from);
                    // log_info("to  : %s", to);
                    case 4:
                    *to++ = *from++;
                    // log_info("from: %s", from);
                    // log_info("to  : %s", to);
                    case 3:
                    *to++ = *from++;
                    // log_info("from: %s", from);
                    // log_info("to  : %s", to);
                    case 2:
                    *to++ = *from++;
                    // log_info("from: %s", from);
                    // log_info("to  : %s", to);
                    case 1:
                    *to++ = *from++;
                    // log_info("from: %s", from);
                    // log_info("to  : %s", to);
                } while (--n > 0);
        }
    }

    return count;
}

/* 
 * a version of duff's device that uses a goto
 * so we can clue in to what's going on with the 
 * weird do-whilte placement in duffs_device
 */
int zeds_device(char *from, char *to, int count)
{
    {
        int n = (count + 7) / 8;
        debug("n starts: %d, count: %d, count: %d", 
                n, count, count % 8);

        switch (count % 8) {
            case 0:
again:    *to++ = *from++;

            case 7:
          *to++ = *from++;
            case 6:
          *to++ = *from++;
            case 5:
          *to++ = *from++;
            case 4:
          *to++ = *from++;
            case 3:
          *to++ = *from++;
            case 2:
          *to++ = *from++;
            case 1:
          *to++ = *from++;
          debug("last case: n=%d", n);
          if (--n > 0) {
              debug("going again: n=%d", n);
              goto again;
          }
        }
    }

    return count;
}

/* 
 * loops over the bytearray, then checks
 * if the ith value is equal.
 * if not, then log_err it in the system.
 */
int valid_copy(char *data, int count, char expects)
{
    int i = 0;
    for (i = 0; i < count; i++) {
        if (data[i] != expects) {
            log_err("[%d] %c != %c", i, data[i], expects);
            return 0;
        }
    }

    return 1;
}

int main(int argc, char *argv[])
{
    char from[53] = { 'a' };
    char to[53] = { 'c' };
    int rc = 0;

    /* this fills up the array with 'x' values
     * from the start of the byte array up until the end
     */
    memset(from, 'x', 53);

    /* this fills the last 3 chars with z values
     * and thus will throw an error
     */
    // memset(from + 1000, 'z', 3);

    // just logs what's going one
    // log_info("value of x: %s", from);

    /* 
     * this fills up the array with 'y' values
     * from the start of the byte array up until 
     * the end
     */
    memset(to, 'y', 53);

    // validates if this is an accurate copy
    check(valid_copy(to, 53, 'y'), "Not initialized right.");

    // use normal copy to 
    rc = normal_copy(from, to, 53);
    check(rc == 53, "Normal copy failed: %d", rc);
    check(valid_copy(to, 53, 'x'), "Normal copy failed.");

    // reset
    memset(to, 'y', 53);

    // duffs version
    rc = duffs_device(from, to, 53);
    check(rc == 53, "Duff's device failed: %d", rc);
    check(valid_copy(to, 53, 'x'), "Duff's device failed copy.");

    // reset
    memset(to, 'y', 53);

    // my version
    rc = zeds_device(from, to, 53);
    check(rc == 53, "Zed's device failed: %d", rc);
    check(valid_copy(to, 53, 'x'), "Zed's device failed copy.");

    return 0;
error:
    return 1;
}
