#include <stdio.h>
#include <stdlib.h>

typedef struct Node
{
    int          x;
    struct Node* next;
} Node;

void insert_end(Node** root, int value)
{
    Node* new_node = malloc(sizeof(Node));
    if (new_node == NULL) { exit(1); }
    new_node->next = NULL;
    new_node->x    = value;

    if (*root == NULL)
    {
        *root = new_node;
        return;
    }

    Node* curr = *root;
    while (curr->next != NULL) { curr = curr->next; }
    curr->next = new_node;
}
// the part i'm reading the file
void deserialize(Node** root)
{
    FILE* file = fopen("text.txt", "r");
    if (file == NULL) { exit(2); }
    int val;
    int val2;
    while (fscanf(file, "%d", &val) > 0)
    {
        insert_end(root, val);
    }
    fclose(file);
}
int main(int argc, char* argv[])
{
    Node* root = NULL;
    if (root == NULL) { exit(2); }
    root->x    = 15;
    root->next = NULL;

    deserialize(&root);

    for (Node* curr = root; curr != NULL; curr = curr->next)
    {
        printf("%d\n", curr->x);
    }

    deallocate(&root);

    return 0;
}