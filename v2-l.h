#pragma once

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

// here is the linked list stuff
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

List* create_l();
List* destroy_l(List*);

int empty(List*);
int insert_n(Info*, List*);
int remove_n(List*);
int show_l(List*, const char*);
int size(List*);

// here goes the program target
List*    deserialize(const char* file);
int      serialize(List* list, const char* file);
double   pri_avg(List*);
