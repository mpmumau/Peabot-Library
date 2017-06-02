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

void *config_get(unsigned short arg_name);
void config_set(unsigned short arg_name, void *val);

int main(int argc, char *argv[])
{
    int arg1_init_val = 74621;
    int arg2_init_val = 25124;

    config_set(ARG1, (void *) &arg1_init_val);
    config_set(ARG2, (void *) &arg2_init_val);

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