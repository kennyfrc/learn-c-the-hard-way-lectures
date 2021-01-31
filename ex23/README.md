The basic idea of loop unrolling is that the number of instructions executed in a loop can be reduced by reducing the number of loop tests, sometimes reducing the amount of time spent in the loop. 

For example, in the case of a loop with only a single instruction in the block code, the loop test will typically be performed for every iteration of the loop, that is every time the instruction is executed. 

If, instead, eight copies of the same instruction are placed in the loop, then the test will be performed only every eight iterations, and this may gain time by avoiding seven tests. However, this only handles a multiple of eight iterations, requiring something else to handle any remainder of iterations.[1]

Duff's device provides a solution by first performing the remainder of iterations, followed by iterating as many times as necessary the multiple of eight similar instructions. 

To determine the number of remainder iterations, the code first calculates the total number of iterations modulo eight. According to this remainder, the program execution will then jump to a case statement followed by exactly the number of iterations needed. Once this is done, everything is straightforward – the code continues by doing iterations of groups of eight instructions, this has become possible since the remaining number of iterations is a multiple of eight.[1]

Duff's device provides a compact loop unrolling by using the case keyword both inside and outside the loop. This is unusual because the contents of a case statement are traditionally thought of as a block of code nested inside the case statement, and a reader would typically expect it to end before the next case statement. According to the specifications of C language, this is not necessary; indeed, case statements can appear anywhere inside the switch code block, and at any depth; the program execution will simply jump to the next statement, wherever it may be.

