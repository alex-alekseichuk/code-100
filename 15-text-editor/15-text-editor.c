/**
 * gcc 15-text-editor.c -lncurses -ltinfo
 */
#include <ncurses.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <locale.h>
#include <signal.h>
#include <string.h>

typedef struct Buffer {
    char **lines;
    size_t len;
    size_t size;
} Buffer;

void buffer_close(Buffer *buffer) {
    if (buffer->lines) {
        for (int i = 0; i < buffer->len; i++)
            if (buffer->lines[i])
                free(buffer->lines[i]);
        free(buffer->lines);
        buffer->lines = NULL;
        buffer->len = 0;
        buffer->size = 0;
    }
}

int buffer_increase(Buffer *buffer) {
    if (!buffer->lines) return 0;
    int prev_size = buffer->size;
    buffer->size *= 2;
    buffer->lines = realloc(buffer->lines, buffer->size * sizeof(char *));
    if (!buffer->lines) {
        return 0;
    }
    for (int i = prev_size; i < buffer->size; i++)
        buffer->lines[i] = NULL;    
    return 1;
}

int file_open(Buffer *buffer, const char *file_path) {
    buffer_close(buffer);

    FILE *file;

    file = fopen(file_path, "r");
    if (!file) {
        if (access(file_path, F_OK) == -1) {
            buffer->size = 32;
            buffer->len = 0;
            buffer->lines = calloc(buffer->size, sizeof(char *));
            if (!buffer->lines) {
                fprintf(stderr, "Can't allocate memory for the buffer\n");
                return 0;
            }
            for (int i = 0; i < buffer->size; i++)
                buffer->lines[i] = NULL;
            return 1;
        }
        return 0;
    }

    buffer->size = 32;
    buffer->len = 0;
    buffer->lines = malloc(buffer->size * sizeof(char *));
    if (!buffer->lines) {
        fprintf(stderr, "Can't allocate memory for the buffer\n");
        return 0;
    }
    for (int i = 0; i < buffer->size; i++)
        buffer->lines[i] = NULL;

    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    while ((read = getline(&line, &len, file)) != -1) {
        while (buffer->len >= buffer->size) {
            if (!buffer_increase(buffer)) {
                fprintf(stderr, "Can't allocate memory for the buffer\n");
                return 0;
            }
        }

        buffer->lines[buffer->len] = malloc(read + 1);
        if (!buffer->lines[buffer->len]) {
            fprintf(stderr, "Can't allocate memory for the buffer\n");
            return 0;
        }

        strncpy(buffer->lines[buffer->len], line, read);
        buffer->len++;
    }

    free(line);
    fclose(file);

    return 1;
}

int file_save(Buffer *buffer, const char *file_path) {
    FILE *file;

    file = fopen(file_path, "w");
    if (!file) {
        return 0;
    }

    for (int i = 0; i < buffer->len; i++)
        if (buffer->lines[i])
            fputs(buffer->lines[i], file);

            fclose(file);

    return 1;
}

static Buffer buffer = {.lines=NULL};

void exit_app() {
    buffer_close(&buffer);
    int r_end = endwin();
    exit(r_end == ERR);
}

void sigint_handler(int signum) {
    exit_app();
}

static int cols;
static int rows;
static int top_line;
static int x, y;

void show_line(int i_line) {
    mvaddstr(i_line, 0, buffer.lines[i_line + top_line]);
}

void move_cursor() {
    int len = buffer.lines[y + top_line] == NULL ? 0 : strlen(buffer.lines[y + top_line]);
    if (x >= len)
       x = len - 1;
    move(y, x);
}

void show_lines() {
    for (int i = 0; i < rows; i++)
        show_line(i);
    refresh();
    move_cursor();
}

int main(int argc, char *argv[]) {
    signal(SIGINT, sigint_handler);
    signal(SIGTERM, sigint_handler);
    signal(SIGQUIT, sigint_handler);

    if (argc < 2) {
        fprintf(stderr, "Usage: %s <text-file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *file_path = argv[1];
    
    if (!file_open(&buffer, file_path)) {
        fprintf(stderr, "Can't open/create file: %s\n", file_path);
        return EXIT_FAILURE;
    }

    setlocale(LC_ALL, "");
    initscr();
    cols = getmaxx(stdscr);
    rows = getmaxy(stdscr);
    cbreak();
    noecho();
    curs_set(0);

    top_line = 0;
    x = 0;
    y = 0;

    show_lines();

    curs_set(1);

    keypad(stdscr, TRUE);
    set_escdelay(25);
    int key;
    while(key = getch()) {
        if (key == KEY_F(10) || key == 27) {
            break;
        }
        if (key == KEY_F(2)) {
            file_save(&buffer, file_path);
        }
        switch (key) {
            case KEY_UP:
                if (y > 0) {
                    y--;
                    move_cursor();
                } else {
                    if (top_line > 0) {
                        top_line--;
                        show_lines();
                    }
                }
                break;
            case KEY_DOWN:
                if (y < rows - 1) {
                    if (y < buffer.len - top_line - 1) {
                        y++;
                        move_cursor();
                    }
                } else {
                    if (top_line + rows < buffer.len) {
                        top_line++;
                        show_lines();
                    }
                }
                break;
            case KEY_LEFT:
                if (x > 0) {
                    x--;
                    move_cursor();
                }
                break;
            case KEY_RIGHT:
                {
                    int n = buffer.lines[y + top_line] == NULL ? 0 : strlen(buffer.lines[y + top_line]);
                    if (x < cols - 1 && x < n - 1) {
                        x++;
                        move_cursor();
                    }
                }               
                break;
        }
    }

    exit_app();
    return 0;
}
