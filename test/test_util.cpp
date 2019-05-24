#include <stdio.h>
#include <map>
#include "../utils/util.c"


int main()
{
    char id[20] = {0};
    generateUniqueId(id);
    printf("id = %s, id size= %d\n", id, sizeof(id));
    std::map<char*, int> map;

    return 0;
}