// jess.c

#include "jess.h"

typedef struct {
    int length;
    char *content;
} string;

typedef struct {
    int start;
    int end;
} range;

typedef struct rope {
    struct rope *next;
    struct rope *last;
    string *content;
} rope;

typedef struct {
    FILE *descriptor;
    char *filename;
    rope *text;
    rope *current_line;
} file;

string *new_string(char *content, int length) {
    string *s = (string *)malloc(sizeof(string));
    if (length == -1)
        length = strlen(content);
    s->content = content;
    s->length = length;
    return s;
}

rope *new_rope(char *content) {
    rope *r = (rope *)malloc(sizeof(rope));
    r->next = NULL;
    r->last = NULL;
    r->content = new_string(content, -1);
    return r;
}

rope *nth_rope(rope *r, int n) {
    rope *current = r;
    for (int i = 0; i < n; i++) {
        current = current->next;
        if (!current) return NULL;
    }
    return current;
}

void append_line(file *f, char *s) {
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
        append_line(f, strdup(s));
    }
    return f;
}

void print_file(file *f) {
    rope *current_line = f->text;
    printf("----< %s >----\n", f->filename);
    do {
        printf("%s", current_line->content->content);
    } while ((current_line = current_line->next));
    for (int i = 0; i < strlen(f->filename); i++) printf("-");
    printf("------------\n");
}

bool empty_range(range r) {
    return r.start == -2 and r.end == 2;
}

bool single_range(range r) {
    return r.start == r.end and r.start >= 0;
}

range parse_range(char **remaining) {
    int start = -1;
    int end = -1;

    char *input = *remaining;

    if (input[0] == ',') {
        input++;
        goto parse_end_range;
    } else if (IS_DIGIT(input[0])) {
        start = strtol(input, &input, 10);
        if (input[0] == ',') {
            input++;
            goto parse_end_range;
        } else {
            end = start;
            goto final;
        }
    } else {
        // range is (-2, -2) if there is no range
        start = -2;
        end = -2;
        goto final;
    }

parse_end_range:
    if (IS_DIGIT(input[0])) {
        end = strtol(input, &input, 10);
    }
final:
    *remaining = input;
    range r;
    r.start = start;
    r.end = end;
    return r;
}

void set_current_to(file *f, int i) {
    rope *nth = nth_rope(f->text, i);
    f->current_line = nth ? nth : f->current_line;
}

void current_from_range(file *f, range r) {
    if (single_range(r)) set_current_to(f, r.start);
}

void execute_command(file *f, range r, char command, char *remaining) {
    switch (command) {
        case 'a':
            current_from_range(f, r);
        case '\0':
            current_from_range(f, r);
    }
}

int main(int argc, char **argv) {
    int returned;
    file *f = new_file("text.txt");
    print_file(f);

    // current line is start
    set_current_to(f, 0);

    char input[MAX_INPUT_LENGTH];
    while (fgets(input, MAX_INPUT_LENGTH, stdin)) {
        char *remaining = input;
        range r = parse_range(&remaining);
        remaining[strlen(remaining)-1] = '\0';
        execute_command(f, r, remaining[0], remaining+1);
        printf("%s\n", f->current_line->content->content);
    }
}
