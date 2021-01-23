// key ideas
// structs are organized blocks of memory
// when creating, you'll need to return a pointer of type struct
// you'll need to malloc based on the sizeof(struct Object)
// when manipulating structs, you can access its elements with a pointer

// why arrows instead of dots
// The 'arrow' operator is syntactic sugar. bar->member is the same as (*bar).member. 
// - One reason for the difference is maintainability. 
// - With the arrow operator distinct from the dot operator, it becomes much easier to keep track of which variables are pointers and which are not. 
/// - It might be possible to always use . and have the compiler try to do the right thing, but I doubt that would make the language simpler. Trusting the compiler to interpret what you meant instead of what you literally wrote usually turns out badly.


// provides support for input/output capabilities
#include <stdio.h>

// contains assert, which aborts the program if the user-specified condition is not true
#include <assert.h>

// general utilities, covering memory management, string converions, random numbers
#include <stdlib.h>

// general string manipulation
#include <string.h>


// this is an organized block of memory
// functions like a row in a database or an object in OOP
struct Person {
    char *name;
    int age;
    int height;
    int weight;
};

// Person_create returns a pointer of the struct Person type
struct Person *Person_create(char *name, int age, int height,
        int weight)
{
    // malloc returns a pointer
    // *who defines a pointer to a struct Person type
    struct Person *who = malloc(sizeof(struct Person));
    assert(who != NULL);

    // strdup returns a pointer to a duplicate of the string argument
    // arrow operator exists to access the members of the structure / union using pointers
    who->name = strdup(name);
    who->age = age;
    who->height = height;
    who->weight = weight;

    // return the pointer
    return who;
}

void Person_destroy(struct Person *who)
{
    assert(who != NULL);

    // free up the memory to avoid a memory leak
    // this applies for methods that you've used within the create method
    // and it also applies to the struct itself
    free(who->name);
    free(who);
}

// \t is a tab
void Person_print(struct Person *who)
{
    printf("Name: %s\n", who->name);
    printf("\tAge: %d\n", who->age);
    printf("\tHeight: %d\n", who->height);
    printf("\tWeight: %d\n", who->weight);
}

int main(void)
{
    // make two people structures
    struct Person *joe = Person_create("Joe Alex", 32, 64, 140);

    struct Person *frank = Person_create("Frank Blank", 20, 72, 180);

    // print them out and where they are in memory
    printf("Joe is at memory location %p:\n", joe);
    Person_print(joe);

    printf("Frank is at memory location %p:\n", frank);
    Person_print(frank);

    // make everyone age 20 years and print them again
    joe->age += 20;
    joe->height -= 2;
    joe->weight += 40;
    Person_print(joe);

    frank->age += 20;
    frank->weight += 20;
    Person_print(frank);

    // destroy them both so we clean up
    Person_destroy(joe);
    Person_destroy(frank);

    return 0;
}


// EXTRA CREDIT
// • How to create a struct on the stack, just like you’re making any other variable. 
// • How to initialize it using the x.y (period) character instead of the x->y syntax. 
// • How to pass a structure to other functions without using a pointer.