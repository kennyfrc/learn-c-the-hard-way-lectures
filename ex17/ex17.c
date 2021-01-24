// key ideas
// the times where you need to use * and & a lot is when you're passing around
// structs, arrays, and unions
// fwrite, fopenn, errno, fgets, fputs
// designated initializers to dynamically create new structs, unions, and arrays
// have lots of if statements to catch errors
// -> there's an elegant way to do this without the curly brackets
// -> do it before or after. especially when mutating data or when managing inputs

// personal notes
// -> it's probably better to treat the Connection as the class intead of Database
// -> that's because Database_* modifies Connection when Database is part of Connection


// libraries that you need
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

// #define is a way for us to create constants
#define MAX_DATA 512
#define MAX_ROWS 100

// define struct with MAX_DATA
// this is a row in the db
struct Address {
    int id;
    int set;
    char name[MAX_DATA];
    char email[MAX_DATA];
};

// this is the db with multiple rows / addresses
// each row is a block of memory (structs)
struct Database {
    struct Address rows[MAX_ROWS];
};

// to establish a connection, you need a file
// and you need a db that receives the connection
// gtiven that the Database is a struct, you provide a pointer for it
struct Connection {
    FILE *file;
    struct Database *db;
};

// errno is a preprocessor macro that expands to a thread-local modifiable lvalue (locator value) of type int
// several standard library functions indicate errors by writing positive integers to errno. Typically, the value of errno is set to one of the error codes listed in <errno.h> as macro constants beginning with the letter E followed by uppercase letters or digits.
void die(const char *message)
{
    if (errno) {
        perror(message);
    } else {
        printf("ERROR: VALID ACTIONS: c=create, g=get, s=set, d=del, l=list\n");
        printf("ERROR: %s\n", message);
    }
    // Causes normal program termination to occur.
    exit(1);
}

// this takes the pointer of an address then prints id, name, and email
void Address_print(struct Address *addr)
{
    printf("%d %s %s\n", addr->id, addr->name, addr->email);
}

// it grabs the pointer to a connection...
void Database_load(struct Connection *conn)
{
    // Reads up to count objects into the array buffer from the given input stream stream as if by calling fgetc (look at the character) size times (size of struct for example) for each object,
    // Then stores the results, in the order obtained, into the successive positions of buffer, 
    // Thie results are then reinterpreted as an array of unsigned chars
    // fread(void *buffer, size_t size, size_t count, FILE *stream)
    // what is size_t? 
    // -> size_t is the unsigned integer type of the result of sizeof
    // -> In laymen's terms an unsigned int is an integer that can not be negative and thus has a higher range of positive values that it can assume. A signed int is an integer that can be negative but has a lower positive range in exchange for more negative values it can assume.
    int rc = fread(conn->db, sizeof(struct Database), 1, conn->file);
    if (rc != 1)
        die("Failed to load database.");
}

struct Connection *Database_open(const char *filename, char mode)
{
    // opening creates a connection
    struct Connection *conn = malloc(sizeof(struct Connection));
    if (!conn)
        die("Memory error");

    // given that db is also a struct, you'll need to alloate for that too
    conn->db = malloc(sizeof(struct Database));
    if (!conn->db)
        die("Memory error");

    // if create, then 
    // create a pointer associated with the file stream then return it
    if (mode == 'c') {
        conn->file = fopen(filename, "w");
    } else {
    // if not write, then just read
        conn->file = fopen(filename, "r+");

    // if the conenction is in the file, then load the db in the connection
        if (conn->file) {
            Database_load(conn);
        }
    }

    // open the file then add it to conn->db
    // this essentially "updates" the connection
    if (!conn->file)
        die("Failed to open the file");

    // once the db of the connection is updated, then return the connection
    return conn;
}

// the pattern seems to be:
// -> release the memory of the individual elements first
// -> then release the main struct
void Database_close(struct Connection *conn)
{
    if (conn) {
        if (conn->file)
            // Closes the given file stream. Any unwritten buffered data are flushed to the OS. Any unread buffered data are discarded.
            // Releases the memory occupied by its I/O buffer.
            fclose(conn->file);
        if (conn->db)
            free(conn->db);
        free(conn);
    }
}

// mutates database
void Database_write(struct Connection *conn)
{
    // Moves the file position indicator to the beginning of the given file stream.
    /// "resets" the position indicator
    rewind(conn->file);

    // takes conn->db (which was set by Database_open) then writes it to conn->file
    int rc = fwrite(conn->db, sizeof(struct Database), 1, conn->file);
    if (rc != 1)
        die("Failed to write database.");

    // For output streams (and for update streams on which the last operation was output), writes any unwritten data from the stream's buffer to the associated output device.
    rc = fflush(conn->file);
    if (rc == -1)
        die("Cannot flush database.");
}

// this mutates rows in conn-db
// set mainly indicates that "this has been written"
void Database_create(struct Connection *conn)
{
    int i = 0;

    for (i = 0; i < MAX_ROWS; i++) {
        // make a prototype to initialize it
        // this is called a designated initializer
        // this creates a new struct of type Address named addr
        // then you assign addr.id to i and addr.set to 0
        struct Address addr = {.id = i, .set = 0 };
        
        // then you assign this addr to each row
        conn->db->rows[i] = addr;
    }
}

// this sets new data into the Database
// this ensures that we don't just overwrite stuff into the database
void Database_set(struct Connection *conn, int id, const char *name,
        const char *email)
{
    struct Address *addr = &conn->db->rows[id];
    if (addr->set)
        die("Already set, delete it first");

    addr->set = 1;

    // if it exceeds limits, return error
    if (strlen(name) >= sizeof(addr->name) || strlen(email) >= sizeof(addr->email))
        die("Name or email size exceeds limit");

    // copy name
    char *res = strncpy(addr->name, name, MAX_DATA);
    if (!res)
        die("Name copy failed");

    // copy email
    res = strncpy(addr->email, email, MAX_DATA);
    if (!res)
        die("Email copy failed");
}

// grab id then print
void Database_get(struct Connection *conn, int id)
{
    struct Address *addr = &conn->db->rows[id];

    if (addr->set == 0)
        die("ID is not set");

    Address_print(addr);
}

// grab id, then set designated initializer to 0, then set it in the db
void Database_delete(struct Connection *conn, int id)
{
    struct Address addr = {.id = id,.set = 0 };
    conn->db->rows[id] = addr;
}

// loop over addresses then print them
// if you want to pass blocks of memory around, you need to pass addresses / pointers
void Database_list(struct Connection *conn)
{
    int i = 0;
    struct Database *db = conn->db;

    for (i = 0; i < MAX_ROWS; i++) {
        struct Address *cur = &db->rows[i];

        if (cur->set) {
            Address_print(cur);
        }
    }
}

int main(int argc, char *argv[])
{
    if (argc < 3)
        die("USAGE: ex17 <dbfile> <action> [action params]");

    // argv[0] is the program itself
    // argv[1] is the first argument, etc.
    char *filename = argv[1];
    char action = argv[2][0];

    // open a file to manage
    // otherwise, it says  No such file or directory (from the stdlib)
    struct Connection *conn = Database_open(filename, action);
    
    int id = 0;

    // check records
    if (argc > 3) id = atoi(argv[3]);
    if (id >= MAX_ROWS) die("There's not that many records.");

    switch (action) {
        case 'c':
            Database_create(conn);
            Database_write(conn);
            break;

        case 'g':
            if (argc != 4)
                die("Need an id to get");

            Database_get(conn, id);
            break;

        case 's':
            if (argc != 6)
                die("Need id, name, email to set");

            Database_set(conn, id, argv[4], argv[5]);
            Database_write(conn);
            break;

        case 'd':
            if (argc != 4)
                die("Need id to delete");

            Database_delete(conn, id);
            Database_write(conn);
            break;

        case 'l':
            Database_list(conn);
            break;
        default:
            die("Invalid action: c=create, g=get, s=set, d=del, l=list");
    }

    Database_close(conn);

    return 0;
}
