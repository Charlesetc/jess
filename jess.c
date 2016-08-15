// jess.c

#include "jess.h"

typedef struct {
    int length;
    char *content;
} string;

// a linked-list of strings
typedef struct rope {
    struct rope *next;
    struct rope *last;
    string content;
} rope;

typedef struct {
    FILE *descriptor;
    char *filename;
    rope *text;
    rope *current_line;
} file;

string new_string(char *content, int length) {
    string s;
    if (length == -1)
        length = strlen(content);
    s.content = content;
    s.length = length;
    return s;
}

rope *new_rope(char *content) {
    rope *r = (rope *)malloc(sizeof(rope));
    r->next = NULL;
    r->last = NULL;
    r->content = new_string(content, -1);
    return r;
}

void insert_line(file *f, char *s) {
    rope *r = new_rope(s);
    if (f->current_line) {
        r->next = f->current_line->next;
        r->last = f->current_line;
        f->current_line->next = r;
        f->current_line = r;
    } else {
        f->text = r;
        f->current_line = r;
    }
}

file *new_file(char *filename) {
    FILE *descriptor = fopen(filename, "r");
    file *f = (file *)malloc(sizeof(file));
    f->descriptor = descriptor;
    f->filename = filename;
    char *s = (char *)malloc(MAX_LENGTH + 1);
    while (fgets(s, MAX_LENGTH, descriptor)) {
        insert_line(f, strdup(s));
    }
    return f;
}

void print_file(file *f) {
    rope *current_line = f->text;
    printf("----< %s >----\n", f->filename);
    do {
       printf("%s", current_line->content.content);
    } while ((current_line = current_line->next));
    for (int i = 0; i < strlen(f->filename); i++) printf("-");
    printf("------------\n");
}

int main(int argc, char **argv) { int returned;
    file *f = new_file("text.txt");
    print_file(f);
}
