// key ideas

// pointers are just arrays
// arrays are pointers
// - and this means that you can do both pointer arithmetic and array access to use it
// - when you pass an array to a variable, you're passing the pointer to the start of the array
// - you can increment bits

// pointers are not fully arrays
// - while there are cross-overs, you can't just pass on a pointer address like cur_ages then call sizeof(cur_ages) then expect the size of an array such as sizeof(ages)

// pointers are addresses with a type specifier so you can get the right size of data with it
// - purpose of a pointer is to allow you manually index data into blocks or memory if an array can't do it right
// - in most cases, you just use an array. but there are some cases where you use a pointer where it gives you direct access to a memory block

// practical pointer usage
// - [memory chunk + work with it]ask the OS for a chunk of memory and then use a pointer to work with it. structs and strings can work with this
// - [pass memory chunks to functions] pass large blocks of memory (like structs) to a function that can accept a pointer
// - [take address of function + callbacks] take an address of a function then use it as a dynamic callback
// - [scan complex memory chunks] then convert it into a data structure or parse files

// pointers or arrays?
// - old days: compilers were bad at managing pointers
// - nowadays: compilers convert both pointers and arrays into the same machine code; thus it works well

// quiz
// - type *ptr
// - *ptr
// - *(ptr + i)
// - &thing
// - type *ptr = &thing
// - ptr++

// quiz answers
// - a pointer of type type
// - value of pointer
// - value of pointer + i
// - address of thing
// - address of thing is assigned to a pointer of type type
// - increment the pointer based on its type


// when to *(names) and when to (int *)names and when to int **names = names;

#include <stdio.h>

int main()
{
    // create two arrays we care about
    // arrays are pointers or a location inn the computer's memory where the integers start
    // this is also an address
    // "address of the first integer in ages"
    // "it's an address inside the entire computer"
    // C thinks that your entire cmoputer is one large array of bytes. C layers on top of this massive array of bytes with the concepts of $types and $sizes of those types. 

    // C does the following to arrays:
    // - creates a block of memory
    // - points the names ages to the beginning of that block of memory (an address)
    // - indexes into the block by taking the beginning address then getting an element that is i away from there
    // - converts the address at ages+i to an integer of the right size, such that when we convert it, it returns the value of int at i
    int ages[] = { 23, 43, 12, 89, 2 };
    char *names[] = {
        "Alan", "Frank",
        "Mary", "John", "Lisa"
    };

    // just vars for some for loops - safely get the size of ages
    int count = sizeof(ages) / sizeof(int);
    int i = 0;

    // first way using indexing
    for (i = 0; i < count; i++) {
        printf("%s has %d years alive.\n", names[i], ages[i]);
    }

    printf("---\n");

    // create a pointer that points at cur_age and cur_name | for cur_name, it's ** because it's 2dimensional, i.e. it's a pointer to (a pointer to char)
    // cast to pointer of integer
    // since names is already a pointer, just nest one level deeper
    int *cur_age = (int *)ages;
    char **cur_name = names;

    // second way using pointers
    // loop through ages and names but user pointers plus an offset of i
    // *(cur_name + i) is the same as name[i]
    // *(cur_name + i) is read as the value of (pointer cur_name plus i)
    for (i = 0; i < count; i++) {
        printf("%s is %d years old.\n",
                *(cur_name + i), *(cur_age + i));
        // printf("cur_name: %d\n", cur_name);
        // printf("cur_age: %d\n", cur_age);
    }

    printf("---\n");

    // third way, pointers are just arrays
    for (i = 0; i < count; i++) {
        printf("%s is %d years old again.\n", cur_name[i], cur_age[i]);
    }

    printf("---\n");

    // fourth way with pointers in a stupid complex way
    for (cur_name = names, cur_age = ages;
            // this works because integers are 4 bits
            // chars are 8 bytes
            // if you print the digits, you'll see the differences
            // incrementing cur_name and age will increment by bits
            // ages will point at the start of the array
            // names will also point at the start as well
            (cur_age - ages) < count; cur_name++, cur_age++) {
        printf("%s lived %d years so far.\n", *cur_name, *cur_age);
        printf("cur_name bits: %d\n", cur_name);
        printf("cur_age bits: %d\n", cur_age);
        printf("names bits: %d\n", names);
        printf("ages bits: %d\n", ages);
        printf("cur_age - ages: %ld\n", cur_age - ages);
        printf("cur_name - names: %ld\n", cur_name - names);
    }

    return 0;
}
