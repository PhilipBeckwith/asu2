/*
 * Copyright (C) Mohsen Zohrevandi, 2017
 *
 * Do not share this file with anyone
 */
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include "Parser.h"
using namespace std;

int main (int argc, char* argv[])
{
    int task;

    if (argc < 2)
    {
        cout << "Error: missing argument\n";
        return 1;
    }

    /*
       Note that by convention argv[0] is the name of your executable,
       and the first argument to your program is stored in argv[1]
     */

    task = atoi(argv[1]);

    // TODO: Read the input grammar at this point from standard input
    Parser parser;
    Grammar grammar=parser.getGrammar();

    switch (task) {
        case 1:
            // TODO: perform task 1.
            grammar.task1();
            //print non Terminals
            break;

        case 2:
            // TODO: perform task 2.
            grammar.task2();
            break;

        case 3:
            // TODO: perform task 3.
            grammar.task3();
            break;

        case 4:
            // TODO: perform task 4
            grammar.task4();
            break;

        case 5:
            // TODO: perform task 5.
            grammar.task5();
            break;

        default:
            cout << "Error: unrecognized task number " << task << "\n";
            break;
    }
    return 0;
}
