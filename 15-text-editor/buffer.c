#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <locale.h>
#include <signal.h>
#include <string.h>
#include "buffer.h"

void buffer_close(Buffer *buffer) {
    if (buffer->lines) {
        for (int i = 0; i < buffer->size; i++) {
            if (buffer->lines[i]) {
                if (buffer->lines[i]->line) {
                    free(buffer->lines[i]->line);
                }
                free(buffer->lines[i]);
            }
        }
        free(buffer->lines);
        buffer->lines = NULL;
        buffer->len = 0;
        buffer->size = 0;
    }
    if (buffer->file_path) {
        free(buffer->file_path);
        buffer->file_path = NULL;
    }
}

int buffer_resize(Buffer *buffer, size_t new_size) {
    int prev_size = 0;
    int is_realloc = buffer->size > 0;
    if (new_size == 0) {
        if (buffer->size > 0) {
            prev_size = buffer->size;
            buffer->size *= 2;
        } else {
            buffer->size = 32;
        }
    } else {
        if (buffer->size >= new_size)
            return 1;
        prev_size = buffer->size;
        buffer->size = new_size;
    }
    if (is_realloc) {
        buffer->lines = realloc(buffer->lines, buffer->size * sizeof(Line *));
    } else {
        buffer->lines = calloc(buffer->size, sizeof(Line *));
        buffer->len = 0;
    }
    if (!buffer->lines) {
        return 0;
    }
    for (int i = prev_size; i < buffer->size; i++)
        buffer->lines[i] = NULL;    
    return 1;
}

int buffer_add_line(Buffer *buffer, char *str, size_t read) {
    while (buffer->len >= buffer->size) {
        if (!buffer_resize(buffer, 0)) {
            fprintf(stderr, "Can't allocate memory for the buffer\n");
            return 0;
        }
    }

    Line *line = malloc(sizeof(Line));
    if (!line) {
        fprintf(stderr, "Can't allocate memory for the line\n");
        return 0;
    }

    line->line = malloc(read + 1);
    if (!line->line) {
        fprintf(stderr, "Can't allocate memory for the line\n");
        free(line);
        return 0;
    }

    line->size = read;
    line->len = read;
    strncpy(line->line, str, read);
    line->line[read] = '\0';

    buffer->lines[buffer->len] = line;
    buffer->len++;
    return 1;
}

char *buffer_get_line(Buffer *buffer, size_t line) {
    if (line >= buffer->len)
        return "";
    if (NULL == buffer->lines[line])
        return "";
    return buffer->lines[line]->line;
}

int buffer_line_len(Buffer *buffer, size_t line) {
    if (line >= buffer->len)
        return 0;
    if (NULL == buffer->lines[line])
        return 0;
    return buffer->lines[line]->len;
}

int buffer_open_file(Buffer *buffer, const char *file_path) {
    if (!file_path) return 0;

    buffer_close(buffer);

    FILE *file;

    file = fopen(file_path, "r");
    if (!file) {
        if (access(file_path, F_OK) == -1) {
            if (!buffer_resize(buffer, 0)) {
                fprintf(stderr, "Can't allocate memory for the buffer\n");
                return 0;
            }
            return 1;
        }
        return 0;
    }

    if (!buffer_resize(buffer, 0)) {
        fprintf(stderr, "Can't allocate memory for the buffer\n");
        return 0;
    }
    char *str = NULL;
    size_t len = 0;
    ssize_t read;

    while ((read = getline(&str, &len, file)) != -1) {
        buffer_add_line(buffer, str, read-1);
    }

    free(str);
    fclose(file);

    if (buffer->file_path) free(buffer->file_path);
    buffer->file_path = strdup(file_path);

    return 1;
}

int buffer_save_file(Buffer *buffer, const char *file_path) {
    if (!file_path) file_path = buffer->file_path;
    if (!file_path) return 0;

    FILE *file;

    file = fopen(file_path, "w");
    if (!file) {
        return 0;
    }

    for (int i = 0; i < buffer->len; i++) {
        fputs(buffer_get_line(buffer, i), file);
        fputs("\n", file);
    }

    fclose(file);

    return 1;
}

int line_resize(Line *line, size_t new_size) {
    int prev_size = 0;
    int is_realloc = line->size > 0;
    if (new_size == 0) {
        if (line->size > 0) {
            prev_size = line->size;
            line->size *= 2;
        } else {
            line->size = 32;
        }
    } else {
        if (line->size >= new_size)
            return 1;
        prev_size = line->size;
        line->size = new_size;
    }
    if (is_realloc) {
        line->line = realloc(line->line, line->size + 1);
    } else {
        line->line = malloc(line->size + 1);
        line->len = 0;
    }
    if (!line->line) {
        return 0;
    }
    for (int i = prev_size; i < line->size; i++)
        line->line[i] = '\0';
    return 1;
}

int line_insert_char(Line *line, int pos, char ch) {
    if (pos > line->len) return 0;

    while (line->len >= line->size) {
        if (!line_resize(line, 0)) {
            fprintf(stderr, "Can't allocate memory for the line\n");
            return 0;
        }
    }

    if (pos < line->len) {
        memmove(&line->line[pos + 1], &line->line[pos], line->len - pos);
    }

    line->line[pos] = ch;
    line->len++;

    return 1;
}

int line_delete_char(Line *line, int pos) {
    if (line->len == 0 || pos >= line->len) return 0;
    if (pos < line->len - 1)
        memmove(&line->line[pos], &line->line[pos + 1], line->len - pos - 1);
    line->len--;
    line->line[line->len] = '\0';
    return 1;
}

int line_backspace_char(Line *line, int pos) {
    if (pos == 0 || pos > line->len) return 0;
    if (pos < line->len)
        memmove(&line->line[pos-1], &line->line[pos], line->len - pos);
    line->len--;
    line->line[line->len] = '\0';
    return 1;
}

int buffer_join_lines(Buffer *buffer, int i_line) {
    if (i_line >= buffer->len - 1) return -1;
    int new_x = buffer->lines[i_line]->len;

    int new_len = buffer->lines[i_line]->len + buffer->lines[i_line + 1]->len;
    if (buffer->lines[i_line]->len > 0 && buffer->lines[i_line + 1]->len > 0) {
        new_len++;
    }
    if (!line_resize(buffer->lines[i_line], new_len)) return -1;
    if (buffer->lines[i_line + 1]->len > 0) {
        if (buffer->lines[i_line]->len > 0)
            buffer->lines[i_line]->line[buffer->lines[i_line]->len++] = ' ';
        memmove(&buffer->lines[i_line]->line[buffer->lines[i_line]->len], buffer->lines[i_line + 1]->line, buffer->lines[i_line + 1]->len + 1);
    }
    buffer->lines[i_line]->len = new_len;

    free(buffer->lines[i_line + 1]->line);
    buffer->lines[i_line + 1]->line = NULL;
    free(buffer->lines[i_line + 1]);
    buffer->lines[i_line + 1] = NULL;
    if (i_line < buffer->len - 1)
        memmove(&buffer->lines[i_line + 1], &buffer->lines[i_line + 2], sizeof(Line*) * (buffer->len - i_line - 2));
    buffer->len--;
    buffer->lines[buffer->len] = NULL;

    return new_x;
}

int buffer_split_line(Buffer *buffer, int i_line, int pos) {
    while (buffer->len >= buffer->size) {
        if (!buffer_resize(buffer, 0)) {
            fprintf(stderr, "Can't allocate memory for the buffer\n");
            return 0;
        }
    }

    if (i_line < buffer->len - 1)
        memmove(&buffer->lines[i_line + 2], &buffer->lines[i_line + 1], sizeof(Line*) * (buffer->len - i_line - 1));

    buffer->lines[i_line + 1] = malloc(sizeof(Line));
    if (pos < buffer->lines[i_line]->len) {
        int size = buffer->lines[i_line]->len - pos;
        buffer->lines[i_line + 1]->size = size;
        buffer->lines[i_line + 1]->len = size;
        buffer->lines[i_line + 1]->line = malloc(size + 1);
        strncpy(buffer->lines[i_line + 1]->line, &buffer->lines[i_line]->line[pos], size);
        buffer->lines[i_line + 1]->line[size] = '\0';

        buffer->lines[i_line]->len = pos;
        buffer->lines[i_line]->line[pos] = '\0';
    } else {
        buffer->lines[i_line + 1]->size = 0;
        buffer->lines[i_line + 1]->len = 0;
        buffer->lines[i_line + 1]->line = malloc(1);
        buffer->lines[i_line + 1]->line[0] = '\0';
    }

    buffer->len++;
    return 1;
}
