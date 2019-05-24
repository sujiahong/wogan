#include <stdio.h>
#include <map>
#include "../utils/util.c"
#include <pthread.h>

std::map<pthread_t, int> map;
const unsigned int len = 10000000;
char* idArr[5];

void* test(void* args){
    char* allId = idArr[map[pthread_self()]];
    printf("Hello thread %d ，， %u\n", map[pthread_self()], allId);
    for (unsigned int i = 0; i < 40*len; i += 40){
        generateUniqueId(&allId[i]);
        //printf("id %s \n", &allId[i]);
    }
    pthread_exit(NULL);
    return 0;
}

int main()
{
    pthread_t tids[5];
    printf("主pthreadid = %lu\n", pthread_self());
    int ret;
    int k = 0;
    for (; k < 5; ++k){
        idArr[k] = (char*)malloc(40*len);
        ret = pthread_create(&tids[k], NULL, test, NULL);
        if (ret != 0)
            printf("k = %d, pthread_create error: errcode= %d\n", k, ret);
        map[tids[k]] = k;
    }
    for (k = 0; k < 5; ++k){
        //printf("arr addr = %u\n", idArr[k]);
        pthread_join(tids[k], NULL);
    }
    struct info{
        int idx;
        int count;
    };
    std::map<std::string, struct info> cimap;
    for(k = 0; k < 5; ++k){
        char* ids = idArr[k];
        for (int i = 0; i < 40*len; i += 40){
            if (cimap[&ids[i]].count == 0){
                cimap[&ids[i]].idx = k;
                cimap[&ids[i]].count++;
            }else{
                cimap[&ids[i]].count++;
                if (cimap[&ids[i]].count > 1){
                    printf("k = %d, id = %s , map[id].idx= %d, map[id].count= %d\n", k, &ids[i], cimap[&ids[i]].idx, cimap[&ids[i]].count);
                }
            }
            //printf("k = %d, i = %d, id = %s \n",k, i, &ids[i]);
        }
        free(ids);
    }
    return 0;
}