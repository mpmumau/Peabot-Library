/*
 File:          arg_parser.c
 Description:   Implementation of argument parsing functions.
 */

/* Standard library includes */
#include <stdio.h>
#include <getopt.h>

/* Header */
#include "arg_parser.h"

void argparser_parse(int argc, char *argv[])
{
    printf("We rocking now!\n");

    const char *shorts;
    const struct option *longs;
    int *index;

    int val = getopt_long(argc, argv, shorts, longs, index);

    printf("Returned this: %d\n", val);

}