May be you could write this in 2 steps. First try to consume the data on file, then put the code to insert this into a linked list..

About the `struct`

```C
typedef struct Node
{
    int          x;
    struct Node* next;
} Node;
```
maybe not the best way to describe a linked list. This is just a node and not a list. You will be better served with a bit of encapsulation. A list has metadata and pointers to nodes. Nodes points to or contain data. A list is NOT a node. A node is NOT a list. Program a linked list with just a node is problematic at best. Many loose pointers and your case even more problematic `Node**` pointers.

Compare with something like

```C

typedef struct
{
    unsigned burst;
    unsigned arrival;
    unsigned priority;
}   Info;

typedef struct st_node
{
    Info* info;
    st_node* next;
    st_node* prev;
}   Node;

typedef struct
{
    Node* head;
    Node* tail;
    unsigned size;
}   List;

List* create();
List* destroy(List* list);
int insert(Info* one_item, List* list);

```

And see that a list is a list of nodes. Nodes points to info and `info` is the unit of data. Any data.

`insert()` inserts an item into a list, `create()` and `destroy()` manages lists and the encapsulation makes everything far more easier than just using pointers and pointers to pointers.

### Consuming the file ###

Using this file as `input.txt`

```none
2:101:34
20:10:3
5:1:4
```

and this data unit

```C

typedef struct
{
    unsigned burst;
    unsigned arrival;
    unsigned priority;
}   Info;
```

See that the input is a CSV --- from Comma Separated Values --- file, a format from de '70s. The separator is a ':'.

`scanf()` and family was written for this: Scan Formatted Files, hence the name. It is a scanner. So it is easier to just use it in this way.

See this example

```C
List* deserialize(const char* file)
{
    FILE* in = fopen(file, "r");
    if (in == NULL) return NULL;

    Info info = {};
    while (3 == fscanf(
                    in, "%d:%d:%d", &info.arrival,
                    &info.burst, &info.priority))
    {
        fprintf(
            stderr, "%d:%d:%d\n", info.arrival, info.burst,
            info.priority);
    };
    fclose(in);
}
```

and the output

```none
2:101:34
20:10:3
5:1:4
```

when called as 

```C
    deserialize("input.txt");
```

as expected.

### The complete code ###

```C
#include <stdio.h>

typedef struct
{
    unsigned burst;
    unsigned arrival;
    unsigned priority;
}   Info;

typedef struct st_node
{
    Info* info;
    st_node* next;
    st_node* prev;
}   Node;

typedef struct
{
    Node* head;
    Node* tail;
    unsigned size;
}   List;

int insert(Info* one_item, List* list);
List* create();

List* deserialize(const char* file)
{
    FILE* in = fopen(file, "r");
    if (in == NULL) return NULL;

    Info info = {};
    while (3 == fscanf(
                    in, "%d:%d:%d", &info.arrival,
                    &info.burst, &info.priority))
    {
        fprintf(
            stderr, "%d:%d:%d\n", info.arrival, info.burst,
            info.priority);
    };
    fclose(in);
}

int main(void)
{
    deserialize("input.txt");
    return 0;
}
```



