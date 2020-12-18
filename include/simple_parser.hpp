#ifndef SIMPLE_PARSER_H
#define SIMPLE_PARSER_H

extern char* cmd_arg;

enum argument{
    NO_ARGUMENT,
    REQUIRED,
    OPTIONAL
};

typedef struct options 
{
    const char option;
    enum argument arg;
} options;

int parse(int argc, char *argv[], options *opt);
int find_option(char option, options *opt);

#endif
