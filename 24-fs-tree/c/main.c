#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <dirent.h>

/* UTF-8 box-drawing characters used for the tree output.
 * Written as byte escapes so the source stays portable and unambiguous. */
#define CH_CORNER_T "\xe2\x94\x9c" /* ├ */
#define CH_CORNER_L "\xe2\x94\x94" /* └ */
#define CH_H        "\xe2\x94\x80" /* ─ */
#define CH_V        "\xe2\x94\x82" /* │ */
#define CH_HHH      "\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80" /* ─── */

/* Print a single tree entry. The root is never printed.
 * Returns 0 on success, -1 on error. */
static int showRecord(FILE *out, const char *name, long size, const char *prefix,
                      int isRoot, int isLast, int isDir) {
    if (isRoot) {
        return 0;
    }

    const char *first = isLast ? CH_CORNER_L : CH_CORNER_T;

    char suffix[64];
    if (isDir) {
        suffix[0] = '\0';
    } else if (size > 0) {
        snprintf(suffix, sizeof(suffix), " (%ldb)", size);
    } else {
        strcpy(suffix, " (empty)");
    }

    if (fprintf(out, "%s%s%s%s%s\n", prefix, first, CH_HHH, name, suffix) < 0) {
        return -1;
    }
    return 0;
}

/* Sort comparator: byte-wise (the same order as Go's string '<'). */
static int cmp_names(const void *a, const void *b) {
    const char *const *x = (const char *const *)a;
    const char *const *y = (const char *const *)b;
    return strcmp(*x, *y);
}

static char *xstrdup(const char *s) {
    char *r = strdup(s);
    if (!r) {
        fprintf(stderr, "out of memory\n");
    }
    return r;
}

static void free_names(char **names, size_t count) {
    for (size_t i = 0; i < count; i++) {
        free(names[i]);
    }
    free(names);
}

/* Build the last path component (name after the final '/'). */
static const char *last_component(const char *path) {
    const char *slash = strrchr(path, '/');
    return slash ? slash + 1 : path;
}

/* Join parent and child with a single '/' separator. */
static char *join_path(const char *parent, const char *name) {
    size_t plen = strlen(parent);
    size_t nlen = strlen(name);
    int need_sep = (plen > 0 && parent[plen - 1] != '/');
    size_t total = plen + (need_sep ? 1 : 0) + nlen;
    char *r = malloc(total + 1);
    if (!r) {
        fprintf(stderr, "out of memory\n");
        return NULL;
    }
    memcpy(r, parent, plen);
    size_t off = plen;
    if (need_sep) {
        r[off++] = '/';
    }
    memcpy(r + off, name, nlen);
    r[off + nlen] = '\0';
    return r;
}

/* Forward declaration: processDir needs processFile, defined below. */
static int processFile(FILE *out, int printFiles, const char *path,
                       const char *prefix, int isRoot, int isLast);

/* Read a directory, sort its entries, filter (only dirs unless printFiles),
 * and recurse into each. Mirrors Go's processDir. */
static int processDir(FILE *out, int printFiles, const char *path,
                      const char *prefix, int isRoot, int isLast) {
    DIR *d = opendir(path);
    if (!d) {
        fprintf(stderr, "readdir %s: %s\n", path, strerror(errno));
        return -1;
    }

    char **names = NULL;
    size_t count = 0, cap = 0;

    struct dirent *ent;
    while ((ent = readdir(d)) != NULL) {
        const char *name = ent->d_name;
        if (strcmp(name, ".") == 0 || strcmp(name, "..") == 0) {
            continue;
        }
        if (!printFiles) {
            char *full = join_path(path, name);
            if (!full) {
                closedir(d);
                free_names(names, count);
                return -1;
            }
            struct stat st;
            int keep;
            if (stat(full, &st) == 0) {
                keep = S_ISDIR(st.st_mode);
            } else {
                keep = 1; /* unknown: keep it, stat will surface the error */
            }
            free(full);
            if (!keep) {
                continue;
            }
        }
        if (count == cap) {
            cap = cap ? cap * 2 : 16;
            char **tmp = realloc(names, cap * sizeof(char *));
            if (!tmp) {
                closedir(d);
                free_names(names, count);
                fprintf(stderr, "out of memory\n");
                return -1;
            }
            names = tmp;
        }
        char *dup = xstrdup(name);
        if (!dup) {
            closedir(d);
            free_names(names, count);
            return -1;
        }
        names[count++] = dup;
    }
    closedir(d);

    qsort(names, count, sizeof(char *), cmp_names);

    /* Compute the prefix to hand to the children of this directory. */
    char *childPrefix;
    if (isRoot) {
        childPrefix = xstrdup("");
    } else if (isLast) {
        childPrefix = malloc(strlen(prefix) + 2 + 1);
        if (childPrefix) {
            sprintf(childPrefix, "%s\t", prefix);
        }
    } else {
        childPrefix = malloc(strlen(prefix) + strlen(CH_V) + 2 + 1);
        if (childPrefix) {
            sprintf(childPrefix, "%s%s\t", prefix, CH_V);
        }
    }
    if (!childPrefix) {
        free_names(names, count);
        fprintf(stderr, "out of memory\n");
        return -1;
    }

    int status = 0;
    for (size_t i = 0; i < count; i++) {
        int thisIsLast = (i == count - 1);
        char *full = join_path(path, names[i]);
        if (!full) {
            free_names(names, count);
            free(childPrefix);
            return -1;
        }
        status = processFile(out, printFiles, full, childPrefix, 0, thisIsLast);
        free(full);
        if (status != 0) {
            free_names(names, count);
            free(childPrefix);
            return status;
        }
    }

    free_names(names, count);
    free(childPrefix);
    return status;
}

/* Open/stat a path and dispatch to a leaf (file) or a sub-tree (dir).
 * Mirrors Go's processFile. */
static int processFile(FILE *out, int printFiles, const char *path,
                       const char *prefix, int isRoot, int isLast) {
    struct stat st;
    if (stat(path, &st) != 0) {
        fprintf(stderr, "stat %s: %s\n", path, strerror(errno));
        return -1;
    }
    const char *name = last_component(path);

    if (S_ISREG(st.st_mode)) {
        return showRecord(out, name, (long)st.st_size, prefix, isRoot, isLast, 0);
    }
    if (S_ISDIR(st.st_mode)) {
        if (showRecord(out, name, 0, prefix, isRoot, isLast, 1) != 0) {
            return -1;
        }
        return processDir(out, printFiles, path, prefix, isRoot, isLast);
    }

    /* Other types (symlinks, sockets, ...) are skipped, like in the Go code. */
    return 0;
}

/* Entry point used by the tests and by main. Mirrors Go's dirTree. */
int dirTree(FILE *out, const char *path, int printFiles) {
    return processFile(out, printFiles, path, "", 1, 1);
}

/* Wrapped so that main_test.c can reuse the implementation without a second
 * main. Build the CLI normally; build the test with -DTREE_NO_MAIN. */
#ifndef TREE_NO_MAIN

int main(int argc, char **argv) {
    if (argc != 2 && argc != 3) {
        fprintf(stderr, "usage: ./tree . [-f]\n");
        return 1;
    }
    const char *path = argv[1];
    int printFiles = (argc == 3 && strcmp(argv[2], "-f") == 0);

    if (dirTree(stdout, path, printFiles) != 0) {
        return 1;
    }
    return 0;
}

#endif /* TREE_NO_MAIN */
