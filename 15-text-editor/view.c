#include <ncurses.h>
#include <locale.h>
#include "buffer.h"

static int cols;
static int rows;
static int top_line;
static int x, y;

void view_init() {
    setlocale(LC_ALL, "");
    initscr();
    cols = getmaxx(stdscr);
    rows = getmaxy(stdscr);

    cbreak();
    noecho();
    curs_set(0);
}

int view_close() {
    return endwin() == ERR;
}

static Buffer *buffer;

void clear_line(int i_line) {
    move(i_line, 0);
    clrtoeol();
}
void show_line(int i_line) {
    mvaddstr(i_line, 0, buffer_get_line(buffer, i_line + top_line));
}

void show_cursor() {
    if (y >= buffer->len - top_line) y = buffer->len - top_line - 1;
    int len = buffer_line_len(buffer, y + top_line);
    if (x > len)
       x = len;
    move(y, x);
}

void show_view() {
    erase();
    for (int i = 0; i < rows; i++)
        show_line(i);
    refresh();
    show_cursor();
}

void view_buffer(Buffer *_buffer) {
    buffer = _buffer;

    top_line = 0;
    x = 0;
    y = 0;

    show_view();
    curs_set(1);
}

void cmd_up() {
    if (y > 0) {
        y--;
        show_cursor();
    } else {
        if (top_line > 0) {
            top_line--;
            show_view();
        }
    }
}

void cmd_down() {
    if (y < rows - 1) {
        if (y < buffer->len - top_line - 1) {
            y++;
            show_cursor();
        }
    } else {
        if (top_line + rows < buffer->len) {
            top_line++;
            show_view();
        }
    }
}

void cmd_left() {
    if (x > 0) {
        x--;
        show_cursor();
    }
}

void cmd_right() {
    int n = buffer_line_len(buffer, y + top_line);
    if (x < cols - 1 && x < n) {
        x++;
        show_cursor();
    }
}

void cmd_begin_line() {
    if (x == 0) return;
    x = 0;
    show_cursor();
}

void cmd_end_line() {
    int n = buffer_line_len(buffer, y + top_line);
    if (x == cols - 1 || x == n) return;
    x = n;
    show_cursor();
}

void cmd_page_up() {
    if (top_line > 0) {
        top_line -= rows - 1; 
        if (top_line < 0) top_line = 0;
    } else if (y > 0) {
        y = 0;
    } else {
        return;
    }
    show_view();
}

void cmd_page_down() {
    if (buffer->len > top_line + rows) {
        top_line += rows - 1;
        if (top_line > buffer->len - rows) top_line = buffer->len - rows;
    } else if (y < rows - 1 && y < buffer->len - top_line) {
        y = rows - 1;
        if (y + top_line >= buffer->len) y = buffer->len - top_line - 1;
    } else {
        return;
    }
    show_view();
}

void cmd_insert_char(char ch) {
    if (!line_insert_char(buffer->lines[y + top_line], x, ch)) return;
    show_line(y);
    if (x < cols - 1) {
        x++;
    }
    show_cursor();
}

void cmd_delete_char() {
    int n = buffer_line_len(buffer, y + top_line);
    if (x == n) {
        if (y + top_line + 1 >= buffer->len) return;
        int new_x = buffer_join_lines(buffer, y + top_line);
        if (new_x == -1) return;
        show_view();
        return;
    }
    if (!line_delete_char(buffer->lines[y + top_line], x)) return;
    clear_line(y);
    show_line(y);
    show_cursor();
}

void cmd_backspace_char() {
    if (x == 0) {
        if (y + top_line == 0) return;
        int new_x = buffer_join_lines(buffer, y + top_line - 1);
        if (new_x == -1) return;
        x = new_x;
        if (y > 0) y--; else top_line--;
        show_view();
        return;
    }
    if (!line_backspace_char(buffer->lines[y + top_line], x)) return;
    x--;
    clear_line(y);
    show_line(y);
    show_cursor();
}

void cmd_enter() {
    if (!buffer_split_line(buffer, y + top_line, x)) return;
    if (y == rows - 1) top_line++; else y++;
    x = 0;  
    show_view();
    show_cursor();
}
