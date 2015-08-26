#include <math.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main (int argc, char* argv[]) {

    srand(time(NULL));
    while (1) {
        tan (rand());
    }
    return 0;
}

