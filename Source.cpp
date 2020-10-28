#include <stdio.h>
#include <iostream>
#include "scanner.h"

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        fprintf_s(stdout, "Bad input\nUsage: Scanner.exe <path_to_input_file>\n");
    }
    StartScanning(argv[1]);
    system("pause");
    return 0;
}