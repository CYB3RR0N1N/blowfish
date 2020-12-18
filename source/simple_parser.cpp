#include "simple_parser.hpp"

char *cmd_arg;

int find_option(char option, options *opt)
{
    for (int i = 0 ; opt[i].option; i++)
    {
        if (option == opt[i].option)
            return i;
    }
    return -1;
}



int parse(int argc, char *argv[], options *opt)
{
    static int i = 1;
    while(i < argc)
    {
        char *tmp = argv[i];
        if (tmp[0] == '-')
        {
            int num = find_option(tmp[1], opt);
            if (num != -1)
            {
                switch (opt[num].arg)
                {
                    case NO_ARGUMENT:
                        i++;
                        return tmp[1];
                        break;
                    case OPTIONAL: case REQUIRED:
                        if ( (i != argc-1) && (argv[i + 1][0] != '-'))
                        {
                             cmd_arg = argv[i + 1];
                             i+=2;
                             return tmp[1];
                        }
                        else
                        {
                            i++;
                            if (opt[num].arg == REQUIRED)
                                return '?';
                            else 
                                return '!';
                        }    
                        break;
                }
                i++;   
                return tmp[1];
            }
            else
            {
                i++;
                return '?';
            }
        }
        else
        {
            i++;
            cmd_arg = tmp;
            return '@';
        }
        
    }
    i = 0;
    return -1;
}