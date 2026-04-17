#pragma once

void view_init();
void view_buffer(Buffer *_buffer);
int view_close();

void cmd_up();
void cmd_down();
void cmd_left();
void cmd_right();
void cmd_begin_line();
void cmd_end_line();
void cmd_page_up();
void cmd_page_down();
void cmd_insert_char(char ch);
void cmd_delete_char();
void cmd_backspace_char();
void cmd_enter();
