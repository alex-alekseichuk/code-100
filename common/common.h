#pragma once

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

size_t read_line(char **lineptr, size_t *n, FILE *stream);
size_t parse_line(char *str, char *tokens[], size_t *count, const char *delimiters);
void print_log(const char *format, ...);
