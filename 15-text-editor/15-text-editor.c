/**
 * gcc 15-text-editor.c buffer.c view.c controller.c -lncurses -ltinfo
 */
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <locale.h>
#include <signal.h>
#include <string.h>
#include "buffer.h"
#include "view.h"
#include "controller.h"

static Buffer buffer = {.lines=NULL, .file_path=NULL};

void exit_app() {
    buffer_close(&buffer);
    exit(view_close());
}

void sigint_handler(int signum) {
    exit_app();
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
    
    if (!buffer_open_file(&buffer, file_path)) {
        fprintf(stderr, "Can't open/create file: %s\n", file_path);
        return EXIT_FAILURE;
    }

    view_init();
    view_buffer(&buffer);

    int result = ctrl_run(&buffer);

    exit_app();
    return result;
}
