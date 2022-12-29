## Now what? ##

Now we have

```C
List* deserialize(const char* file)
```
that can consume the data from a file like this

`input.txt:`
```none
3:2:1
6:4:2
12:8:3
24:16:4
```

by using

```C
    List* new_list = deserialize("input.txt");
```

But `deserialize()` just prints the data on `stderr`

We want a linked list of the dataset.

**I will write an example below, but step by step in order to cover other cases for other readers.**

### A linked list ###

I will add a sequence number to the nodes just to help in testing.

The linked list has nothing to do with our problem, or else we will need one implementation of linked list for every program in life. In fact would be great if the code for the list was in another source code file, in order to be used anywhere else.

### this is the example for `List` ###

```C
typedef struct st_node
{
    int             num;
    Info*           info;
    struct st_node* next;
    struct st_node* prev;
} Node;

typedef struct
{
    size_t size;
    Node*  head;
    Node*  tail;
} List;
``` 

A list of nodes, nodes pointing to `info`.

`info` is from now on

```C
// info is the thing in the list
typedef struct
{
    unsigned burst;
    unsigned arrival;
    unsigned priority;
    unsigned seq;  // sequence number for testing
} Info;
// this is a helper to format a single node listing
int show_i(Info*, const char*);

```

The code for `show_i()` is simple

```C
int show_i(Info* info, const char* msg)
{
    if (info == NULL) return -1;
    if (msg != NULL) printf("%s", msg);
    printf(
        "#%4d: B:%4d A:%4d P:%4d\n", info->seq, info->burst,
        info->arrival, info->priority);
    return 0;
}
```

And the reason for this to exist is to provided some encapsulation on the way the nodes are printed.

And the functions we will use here are the obvious ones:

```C
List* create_l();
List* destroy_l(List*);

int empty(List*);
int insert_n(Info*, List*);
int remove_n(List*);
int show_l(List*, const char*);
int size(List*);
```

This is a very simple example so we will assume a standard queue, **FIFO** (**Fi**rst In **F**irst **O**ut) one, with items added at the end and removed from the front.
  
  - `empty()` returns 1 if the list is empty
  - `size()` returns the expected size
  - `show_l()` shows the list contents with an optional title message
  - the other functions work as expected. 

### A simple `C` implementation of the linked list ###

Functions have no more than 10 to 15 lines 

```C
List* destroy_l(List* L)
{
    if (L == NULL) return NULL;
    Node* p = L->head;
    for (size_t i = 0; i < L->size; i += 1)
    {  // remove one by one
        Node* nx = p->next;
        free(p->info);  // free data
        free(p);        // free node
        p = nx;
    };            // for
    free(L);      // free list
    return NULL;  // to invalidate pointer
}

int empty(List* L)
{
    if (L == NULL) return 0;
    return (L->size == 0);
}

List* create_l()
{
    List* nv = (List*)malloc(sizeof(List));
    if (nv == NULL) return NULL;
    nv->size = 0;  // vazia
    nv->head = NULL;
    nv->tail = NULL;
    return nv;
}

int insert_n(Info* info, List* L)
{  // inserts at the end of list
    static unsigned seq = 1000;
    if (L == NULL) return -1;
    // new node here
    Node* nv = (Node*)malloc(sizeof(Node));
    // new data here: always copy
    nv->info      = (Info*)malloc(sizeof(Info));
    *(nv->info)   = *info;
    nv->info->seq = seq++;  // USN
    nv->prev      = L->tail;
    nv->next      = NULL;
    // ajusta os ponteiros da lista
    L->size += 1;  // conta o novo
    if (L->size == 1)
        L->head = nv;
    else { L->tail->next = nv; }
    L->tail = nv;
    return (int)L->size;
}

int remove_n(List* L)
{  // remove from start
    if (L == NULL) return -1;
    if (L->size == 0) return -2;
    Node* p = L->head->next;
    free(L->head->info);  // data
    free(L->head);        // node
    L->head = p;
    L->size -= 1;
    if (L->size == 0) L->tail = NULL;
    return (int)L->size;
}

int show_l(List* L, const char* tit)
{
    if (L == NULL) return -1;
    if (tit != NULL) printf("%s", tit);
    if (L->size == 0)
        printf("  no elements\n");
    else
        printf("  %zd elements:\n", L->size);
    if (L->head != NULL)
        printf("  [First seq: %d", L->head->info->seq);
    if (L->tail != NULL)
        printf("  Last seq: %d]\n", L->tail->info->seq);

    Node* p = L->head;
    for (size_t i = 0; i < L->size; i += 1)
    {
        show_i(p->info, "\t");
        p = p->next;
    }
    printf("\n");
    return 0;
}

int size(List* L)
{
    if (L == NULL) return 0;
    return (int)L->size;
}
```

### A test program for the list code ###

```C
#include <stdio.h>
#include "v2-l.h"

int main(void)
{
    Info  info = {1, 1, 1, 1};
    List* my_list  = create_l();
    show_l(my_list, "empty list...\n");
    my_list = destroy_l(my_list);

    my_list = create_l();

    // test size
    const int test_size = 3;
    printf("[Testing with %d elements]\n\n\n", test_size);
    for (int i = 0; i < test_size; i += 1)
    {
        info.priority = i;  // just for testing
        insert_n(&info, my_list);
    };
    char message[] = "NNNNNN elements inserted\n";
    sprintf(message, "%d elements inserted\n", test_size);
    show_l(my_list, message);

    int res = 0;

    while (res >= 0)
    {
        printf("\tabout to remove 1st element:\n");
        res = remove_n(my_list);
        printf(
            "\
\tremove_l() returned %d\n\
\tsize() returned %d\n\
\tempty() returned %d\n",
            res, size(my_list), empty(my_list));
        show_l(my_list, "\n  ==> List now:\n");
        if (res < 0) break;
    };  // while()
    show_l(my_list, "On exit\n");
    my_list = destroy_l(my_list);
    return 0;
}
```

The idea: creates a list with `test_size` elements and then remove one by one until error, calling the functions. Then the list is destroyed.

### output from test ###
```none
empty list...
  no elements

[Testing with 3 elements]


3 elements inserted
  3 elements:
  [First seq: 1000  Last seq: 1002]
        #1000: B:   1 A:   1 P:   0
        #1001: B:   1 A:   1 P:   1
        #1002: B:   1 A:   1 P:   2

        about to remove 1st element:
        remove_l() returned 2
        size() returned 2
        empty() returned 0

  ==> List now:
  2 elements:
  [First seq: 1001  Last seq: 1002]
        #1001: B:   1 A:   1 P:   1
        #1002: B:   1 A:   1 P:   2

        about to remove 1st element:
        remove_l() returned 1
        size() returned 1
        empty() returned 0

  ==> List now:
  1 elements:
  [First seq: 1002  Last seq: 1002]
        #1002: B:   1 A:   1 P:   2

        about to remove 1st element:
        remove_l() returned 0
        size() returned 0
        empty() returned 1

  ==> List now:
  no elements

        about to remove 1st element:
        remove_l() returned -2
        size() returned 0
        empty() returned 1

  ==> List now:
  no elements

On exit
  no elements
```

### Using the list on the original program ###

```C
// here goes the program target
List*    deserialize(const char* file);
int      serialize(List* list, const char* file);
double   pri_avg(List*);
```
The next obvious step is to use the 1st program and consume the file, but this time writing the data into a linked list, and then calling `serialize()` to create a new file with the data in the list.
Sure, **both files must have the same data** and the program will test itself.
As you asked, @jonathan pascal, the function `pri_avg()` computes an useless priority average, just to show how to compute something using the data from all nodes in the list.
Note that this is the same logic as in showing the list contents in `show_l()`. This is called a *filter* and in languages like `C` we can just pass a function address to a function that loops over the code, and use the same code to do anything with the dataset. In `C++` is, for example, a `for_each()` function that does just this. All these functions here has the same logic.

### Example implementation of the 3 functions ###

```C
List* deserialize(const char* file)
{
    FILE* in = fopen(file, "r");
    if (in == NULL) return NULL;

    Info  info  = {0};
    List* new_l = create_l();  // new list here
    while (3 == fscanf(
                    in, "%d:%d:%d", &info.burst,
                    &info.arrival, &info.priority))
    {
        // fprintf(
        //     stderr, "%d:%d:%d\n", info.arrival,
        //     info.burst, info.priority);
        insert_n(&info, new_l);  // insert data into list
    };
    fclose(in);
    return new_l;  // returns a list with the data on file
};

int serialize(List* L, const char* file)
{
    if (L == NULL) return -1;
    if (file == NULL)
    {
        printf("Missing file name\n");
        return -2;
    }
    if (L->size == 0)
    {
        printf("Dataset is empty\n");
        return -3;
    }
    FILE* out = fopen(file, "w");
    if (out == NULL) return -3;

    fprintf(
        stderr,
        "serialize(): writing %d elements into \"%s\"\n",
        size(L), file);
    Node* p = L->head;
    for (size_t i = 0; i < L->size; i += 1)
    {
        fprintf(
            out, "%d:%d:%d\n", p->info->burst,
            p->info->arrival, p->info->priority);
        p = p->next;
    }
    fprintf(out, "\n");
    fclose(out);
    fprintf(stderr, "\"%s\" closed\n", file);
    return 0;
}

// get the priority average from the list
double pri_avg(List* L)
{
    if (L == NULL) return -1;
    if (L->size == 0) return 0.;  // easy
    double avg = 0.;
    Node*  p   = L->head;
    for (size_t i = 0; i < L->size; i += 1)
    {  // here we have node data, one
        // at a time
        avg = avg + p->info->priority;
        p   = p->next;
    };
    return (double)avg / size(L);
}
```

### Testing the 3 functions ###
```C
#include <stdio.h>
#include "v2-l.h"

int main(void)
{
    const char* in_file  = "input.txt";
    printf(
        "deserialize(): building list from \"%s\"\n",
        in_file);
    List* my_list = deserialize(in_file);
    show_l(my_list, "  ==> As read from file...\n");

    printf("average priority is %6.2f\n", pri_avg(my_list));

    const char* out_file = "another.txt";
    int         res      = serialize(my_list, out_file);
    printf(
        "serialize(): dumping list into \"%s\" "
        "returned %d\n",
        out_file, res);

    my_list = destroy_l(my_list);
    return 0;
}
```

And we have full circle over the problem: a CSV file is read from disk, a linked list is built, some values are computed, the list is written to disk in another file.

### test output ###

```none
deserialize(): building list from "input.txt"
  ==> As read from file...
  4 elements:
  [First seq: 1000  Last seq: 1003]
        #1000: B:   3 A:   2 P:   1
        #1001: B:   6 A:   4 P:   2
        #1002: B:  12 A:   8 P:   3
        #1003: B:  24 A:  16 P:   4

average priority is   2.50
serialize(): writing 4 elements into "another.txt"
"another.txt" closed
serialize(): dumping list into "another.txt" returned 0
```








