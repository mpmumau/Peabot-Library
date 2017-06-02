#include <stdio.h>

#define ARG1 1
#define ARG2 2

#define ARG1TEST *((int *) config_get(ARG1))
#define ARG2TEST *((int *) config_get(ARG2))

typedef struct Config
{
    int val1;
    int val2;
} Config;

static Config config;

int main(int argc, char *argv[])
{
    config_set(ARG1, 74621);
    config_set(ARG2, 25124);

    int *arg1_p = (int *) config_get(ARG1);
    int *arg2_p = (int *) config_get(ARG2);

    printf("---Config from function.---\n");
    printf("config.val1: %d\n", *arg1_p);
    printf("config.val2: %d\n", *arg2_p);

    printf("---Config from define.---\n");
    printf("config.val1: %d\n", ARG1TEST);
    printf("config.val2: %d\n", ARG2TEST);    

    return 0;
}

void *config_get(unsigned short arg_name)
{
    switch (arg_name)
    {
        case ARG1:
            return (void *) &config.val1;
        case ARG2:
            return (void *) &config.val2;
    }
    
    return NULL;
}

void config_set(unsigned short arg_name, void *val)
{
    switch (arg_name)
    {
        case ARG1:
            config.val1 = *((int *) val);
        case ARG2:
            config.val2 = *((int *) val);            
    }
}