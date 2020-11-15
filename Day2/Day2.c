#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct paper_and_ribbon {
    int paper;
    int ribbon;
};

typedef struct {
    int length;
    int width;
    int height;
} dim_t;

static FILE			*openFileWithPuzzleInput_ensureSuccess(const char *path);
static bool			 get_next_line_from_file(FILE *file_ptr, char **buffer);
static inline int		 max(int, int);
static inline int		 max_of_three(int, int, int);
static inline int		 min(int, int);
static inline int		 min_of_three(int, int, int);
static struct paper_and_ribbon	 getPaperAndRibbonFromLine(char *line);
static void			 handle_fatal_error(void);

int
main(void)
{
    FILE	*file_ptr     = NULL;
    char	*line	      = NULL;
    int		 paper_total  = 0;
    int		 ribbon_total = 0;

    file_ptr = openFileWithPuzzleInput_ensureSuccess("input");

    while (get_next_line_from_file(file_ptr, &line)) {
	struct paper_and_ribbon pnr = getPaperAndRibbonFromLine(line);

	paper_total  += pnr.paper;
	ribbon_total += pnr.ribbon;
    }

    if (file_ptr) fclose(file_ptr);
    if (line) free(line);

    printf("Paper:\t%d\n", paper_total);
    printf("Ribbon:\t%d\n", ribbon_total);

    return 0;
}

static void
handle_fatal_error(void)
{
    fprintf(stderr, "Fatal error  --  program must exit.\n");
    exit(1);
}

static FILE *
openFileWithPuzzleInput_ensureSuccess(const char *path)
{
    FILE *file_ptr;

    if ((file_ptr = fopen(path, "r")) == NULL)
	handle_fatal_error();
    else
	return file_ptr;

    // NOTREACHED
    return file_ptr;
}

static bool
get_next_line_from_file(FILE *file_ptr, char **buffer)
{
    const int line_max_size = 50;

    if (file_ptr == NULL || buffer == NULL)
	handle_fatal_error();

    if (*buffer) {
	free(*buffer);
	*buffer = NULL;
    }

    if ((*buffer = malloc(line_max_size)) == NULL)
	handle_fatal_error();

    return fgets(*buffer, line_max_size, file_ptr) ? true : false;
}

static struct paper_and_ribbon
getPaperAndRibbonFromLine(char *line)
{
    char *token;
    dim_t pkg_dim = {
	.length = -1,
	.width	= -1,
	.height = -1,
    };
    int foo, bar, baz;
    struct paper_and_ribbon pnr = {
	.paper	= 0,
	.ribbon = 0,
    };

    for (int loop_run = 0;; loop_run++) {
	if ((token = strtok(loop_run == 0 ? line : NULL, "x")) == NULL)
	    break; // Inga fler tokens.

	switch (loop_run) {
	case 0:
	    pkg_dim.length = atoi(token);
	    break;
	case 1:
	    pkg_dim.width = atoi(token);
	    break;
	case 2:
	    pkg_dim.height = atoi(token);
	    break;
	default:
	    handle_fatal_error(); // Ska inte nås.
	}
    }

    if (pkg_dim.length < 0 || pkg_dim.width < 0 || pkg_dim.height < 0)
	handle_fatal_error();

    // 2*l*w + 2*w*h + 2*h*l
    foo = pkg_dim.length * pkg_dim.width;
    bar = pkg_dim.width * pkg_dim.height;
    baz = pkg_dim.height * pkg_dim.length;

    pnr.paper = (2*foo) + (2*bar) + (2*baz) + min_of_three(foo, bar, baz);

    pnr.ribbon += (pkg_dim.length * 2);
    pnr.ribbon += (pkg_dim.width * 2);
    pnr.ribbon += (pkg_dim.height * 2);

    pnr.ribbon -= max_of_three(pkg_dim.length, pkg_dim.width, pkg_dim.height) * 2;
    pnr.ribbon += pkg_dim.length * pkg_dim.width * pkg_dim.height;

    return (pnr);
}

static inline int
min(int a, int b)
{
    return (a < b ? a : b);
}

static inline int
max(int a, int b)
{
    return (a > b ? a : b);
}

static inline int
min_of_three(int foo, int bar, int baz)
{
    int quux = min(foo, bar);

    return min(baz, quux);
}

static inline int
max_of_three(int foo, int bar, int baz)
{
    int quux = max(foo, bar);

    return max(baz, quux);
}
