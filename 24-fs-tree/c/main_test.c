/* Test for dirTree, mirroring go/main_test.go.
 *
 * Build and run from this directory (so ../testdata resolves the same way):
 *   cc -std=c11 -Wall -Wextra -o test main.c main_test.c && ./test
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int dirTree(FILE *out, const char *path, int printFiles);

#define TEST_PATH "../testdata"

#define TEST_TMP "/tmp/c_tree_test_out.txt"

/* Box-drawing characters and tab as separate literals so that a hex escape
 * is always bounded by a string-literal boundary (avoiding the greedy
 * \xNN... escape that would swallow a following hex-looking byte). */
#define C_H "\xe2\x94\x80"
#define C_T "\xe2\x94\x9c"
#define C_L "\xe2\x94\x94"
#define C_V "\xe2\x94\x82"
#define HHH "\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80"
#define TAB "\t"

/* Call dirTree, capturing its output into buf. Returns 0 on success. */
static int treeToString(const char *path, int printFiles, char *buf, size_t bufsz) {
    FILE *f = fopen(TEST_TMP, "w");
    if (!f) {
        return -1;
    }
    int r = dirTree(f, path, printFiles);
    fclose(f);
    if (r != 0) {
        return -1;
    }
    f = fopen(TEST_TMP, "r");
    if (!f) {
        return -1;
    }
    size_t n = fread(buf, 1, bufsz - 1, f);
    buf[n] = '\0';
    fclose(f);
    return 0;
}

int main(void) {
    static char buf[1 << 20];
    int failures = 0;

    /* TestTreeFull */
    const char *testFullResult =
        C_T HHH "project\n"
        C_V TAB C_T HHH "file.txt (19b)\n"
        C_V TAB C_L HHH "gopher.png (70372b)\n"
        C_T HHH "static\n"
        C_V TAB C_T HHH "a_lorem\n"
        C_V TAB C_V TAB C_T HHH "dolor.txt (empty)\n"
        C_V TAB C_V TAB C_T HHH "gopher.png (70372b)\n"
        C_V TAB C_V TAB C_L HHH "ipsum\n"
        C_V TAB C_V TAB TAB C_L HHH "gopher.png (70372b)\n"
        C_V TAB C_T HHH "css\n"
        C_V TAB C_V TAB C_L HHH "body.css (28b)\n"
        C_V TAB C_T HHH "empty.txt (empty)\n"
        C_V TAB C_T HHH "html\n"
        C_V TAB C_V TAB C_L HHH "index.html (57b)\n"
        C_V TAB C_T HHH "js\n"
        C_V TAB C_V TAB C_L HHH "site.js (10b)\n"
        C_V TAB C_L HHH "z_lorem\n"
        C_V TAB TAB C_T HHH "dolor.txt (empty)\n"
        C_V TAB TAB C_T HHH "gopher.png (70372b)\n"
        C_V TAB TAB C_L HHH "ipsum\n"
        C_V TAB TAB TAB C_L HHH "gopher.png (70372b)\n"
        C_T HHH "zline\n"
        C_V TAB C_T HHH "empty.txt (empty)\n"
        C_V TAB C_L HHH "lorem\n"
        C_V TAB TAB C_T HHH "dolor.txt (empty)\n"
        C_V TAB TAB C_T HHH "gopher.png (70372b)\n"
        C_V TAB TAB C_L HHH "ipsum\n"
        C_V TAB TAB TAB C_L HHH "gopher.png (70372b)\n"
        C_L HHH "zzfile.txt (empty)\n";

    int r = treeToString(TEST_PATH, 1, buf, sizeof buf);
    if (r != 0) {
        printf("TestTreeFull: FAILED - error\n");
        failures++;
    } else if (strcmp(buf, testFullResult) != 0) {
        printf("TestTreeFull: FAILED - results not match\nGot:\n%s\nExpected:\n%s\n",
               buf, testFullResult);
        failures++;
    } else {
        printf("TestTreeFull: PASS\n");
    }

    /* TestTreeDir */
    const char *testDirResult =
        C_T HHH "project\n"
        C_T HHH "static\n"
        C_V TAB C_T HHH "a_lorem\n"
        C_V TAB C_V TAB C_L HHH "ipsum\n"
        C_V TAB C_T HHH "css\n"
        C_V TAB C_T HHH "html\n"
        C_V TAB C_T HHH "js\n"
        C_V TAB C_L HHH "z_lorem\n"
        C_V TAB TAB C_L HHH "ipsum\n"
        C_L HHH "zline\n"
        TAB C_L HHH "lorem\n"
        TAB TAB C_L HHH "ipsum\n";

    r = treeToString(TEST_PATH, 0, buf, sizeof buf);
    if (r != 0) {
        printf("TestTreeDir: FAILED - error\n");
        failures++;
    } else if (strcmp(buf, testDirResult) != 0) {
        printf("TestTreeDir: FAILED - results not match\nGot:\n%s\nExpected:\n%s\n",
               buf, testDirResult);
        failures++;
    } else {
        printf("TestTreeDir: PASS\n");
    }

    return failures ? 1 : 0;
}
