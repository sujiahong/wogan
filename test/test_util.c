#include <stdio.h>

#include "../utils/util.c"


int main()
{
    char id[30] = {0};
    generateUniqueId(id);
    printf("id = %s, id size= %d\n", id, sizeof(id));
    return 0;
}