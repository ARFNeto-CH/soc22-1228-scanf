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
