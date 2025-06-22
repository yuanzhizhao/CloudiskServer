#include"config.h"

int main()
{
    printf("Hello world\n");
    const char* filename = "../conf/server.cnf";
    HashTable ht;
    initHashTable(&ht);
    readConfig(filename, &ht);

    printHashTable(&ht);

    printf("find: %s\n", (char*)find(&ht, "ip"));

    destroyHashTable(&ht);

    return 0;
}

