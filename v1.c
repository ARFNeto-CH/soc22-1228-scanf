#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    unsigned burst;
    unsigned arrival;
    unsigned priority;
} Info;

typedef struct st_node
{
    Info*    info;
    struct st_node* next;
    struct st_node* prev;
} Node;

typedef struct
{
    Node*    head;
    Node*    tail;
    unsigned size;
} List;

int   insert_n(Info* one_item, List* list);
List* create();
List* destroy(List*);

List* deserialize(const char* file)
{
    FILE* in = fopen(file, "r");
    if (in == NULL) return NULL;

    Info info = {0};
    List* new_l = create(); // new list here
    while (3 == fscanf(
                    in, "%d:%d:%d", &info.arrival,
                    &info.burst, &info.priority))
    {
        fprintf(
            stderr, "%d:%d:%d\n", info.arrival, info.burst,
            info.priority);
        insert_n(&info, new_l); // insert data into list
    };
    fclose(in);
    return new_l; // returns a list with the data on file
}

int main(void)
{
    List* new_list = deserialize("input.txt");
    new_list       = destroy(new_list);
    return 0;
}

int insert_n(Info* one_item, List* list) { return 0; }

List* create()
{
    List* L = (List*)malloc(sizeof(List));
    if (L == NULL) return NULL;
    L->size = 0;
    L->head = NULL;
    L->tail = NULL;
    return L;
}

List* destroy(List* list) { return NULL; }
