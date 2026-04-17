#pragma once
#include <stdlib.h>

typedef struct Line {
    char *line;
    size_t len;
    size_t size;
} Line;

typedef struct Buffer {
    Line **lines;
    size_t len;
    size_t size;
    char *file_path;
} Buffer;

void buffer_close(Buffer *buffer);
int buffer_resize(Buffer *buffer, size_t new_size);
int buffer_add_line(Buffer *buffer, char *str, size_t read);
char *buffer_get_line(Buffer *buffer, size_t line);
int buffer_line_len(Buffer *buffer, size_t line);

int buffer_open_file(Buffer *buffer, const char *file_path);
int buffer_save_file(Buffer *buffer, const char *file_path);

int line_insert_char(Line *line, int pos, char ch);
int line_delete_char(Line *line, int pos);
int line_backspace_char(Line *line, int pos);
int buffer_join_lines(Buffer *buffer, int i_line);
int buffer_split_line(Buffer *buffer, int i_line, int pos);
