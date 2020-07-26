/*
 File:          arg_parser.c
 Description:   Implementation of argument parsing functions.
 */

/* Standard library includes */
#include <stdio.h>
#include <getopt.h>

static int test_flag;

/* Header */
#include "arg_parser.h"

void argparser_parse(int argc, char *argv[])
{
    int c;

    while (1) {
        static struct option long_options[] = 
        {
            {"mumau", no_argument, &test_flag, 1}
        };

        int option_index = 0;

        c = getopt_long (argc, argv, "abc:d:f:", long_options, &option_index);

        if (c == -1)
            break;

        switch (c)
        {
            case 0:
                if (long_options[option_index].flag != 0)
                    break;

                printf ("option %s", long_options[option_index].name);
                if (optarg)
                    printf (" with arg %s", optarg);
                printf ("\n");
                break;

            case 'a':
                puts("option -a\n");
                break;

            default:
                abort();
        }
    }

    if (test_flag)
        puts("verbose flag is set");
}