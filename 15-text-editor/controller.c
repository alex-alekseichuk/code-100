#include <ncurses.h>
#include "buffer.h"
#include "view.h"

int ctrl_run(Buffer *buffer) {
    keypad(stdscr, TRUE);
    set_escdelay(25);

    int key;
    while(key = getch()) {
        if (key == KEY_F(10) || key == 27) {
            break;
        }
        switch (key) {
            case KEY_F(2):
                buffer_save_file(buffer, buffer->file_path);
                break;
            case KEY_UP:
                cmd_up();
                break;
            case KEY_DOWN:
               cmd_down();
                break;
            case KEY_LEFT:
                cmd_left();
                break;
            case KEY_RIGHT:
                cmd_right();
                break;
            case KEY_HOME:
                cmd_begin_line();
                break;
            case KEY_END:
                cmd_end_line();
                break;
            case KEY_PPAGE:
                cmd_page_up();
                break;
            case KEY_NPAGE:
                cmd_page_down();
                break;
            case KEY_DC:
                cmd_delete_char();
                break;
            case KEY_BACKSPACE:
            case 127:
            case 8:
                cmd_backspace_char();
                break;
            case KEY_ENTER:
            case '\n':
            case '\r':
                cmd_enter();
                break;
        }
        if (key >= 32 && key <= 126) {
            cmd_insert_char(key);
        }
    }

    return 0;
}
