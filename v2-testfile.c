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
