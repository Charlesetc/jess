// jess.c

#include "jess.h"

int main(int argc, char **argv) {
    char *input = NULL;
    size_t input_length;
    getline(&input, &input_length, stdin);
    printf("%s\n", input);
}
