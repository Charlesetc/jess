// jess.c

#include "jess.h"

int main(int argc, char **argv) {
    int returned;
    do {
        size_t input_length;
        char *input = NULL;
        returned = getline(&input, &input_length, stdin);
        printf("%s\n", input);
    } while (returned != -1);
}
