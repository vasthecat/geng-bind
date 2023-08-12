#include "gtools.h"
#include <ucontext.h>
#include <stdint.h>
#include <stdbool.h>

// TODO: move that to struct so that this code would be thread-safe (probably)
static unsigned long counter;
static graph *cur;
static int gn;
extern int generate_done;
int iter_done;
ucontext_t geng_worker, geng_user;
char geng_stack[1 << 20];

// TODO: only on macos
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

void
printgraph(graph *g, int n)
{
    set *gi;
    for (int i = 0; i < n; ++i)
    {
        gi = GRAPHROW(g, 1, i);
        for (int j = 0; j < n; ++j)
        {
            int q = ISELEMENT(gi, j);
            printf("%i ", q);
        }
        printf("\n");
    }
    printf("\n");
}

// TODO: probably don't need to name this function with macro
void
OUTPROC(__attribute__((unused)) FILE *outfile, graph *g, int n)
{
    cur = g;
    gn = n;
    ++counter;

    // TODO: add support for generating graphs in batches (for speed)
#if 0
    static int i = 1;
    i++;
    if (i % 100 == 0)
    {
        swapcontext(&geng_worker, &geng_user);
        i = 0;
    }
#else
    swapcontext(&geng_worker, &geng_user);
#endif
}

// TODO: probably don't need to name this function with macro
// TODO: geng.h maybe?
extern int
GENG_MAIN(int argc, char *argv[]);

struct geng_iterator
{
    ucontext_t geng_worker, geng_user;
    char geng_stack[1 << 20];
};

void
geng_iterator_init(__attribute__((unused)) struct geng_iterator *iter, int n)
{
    iter_done = 0;

    // TODO: add support for more arguments
    int geng_argc = 3;
    char **geng_argv;
    geng_argv = malloc((geng_argc + 1) * sizeof(char *));
    geng_argv[0] = "geng";
    geng_argv[1] = "-q";
    char n_str[20];
    snprintf(n_str, sizeof(n_str), "%u", n);
    geng_argv[2] = n_str;
    geng_argv[3] = NULL;

    uint32_t p_argv[2];
    p_argv[0] = (uint32_t) (((size_t) &geng_argv) & ((1llu << 32) - 1llu));
    p_argv[1] = ((size_t) &geng_argv) >> 32;

    counter = 0;

    getcontext(&geng_user);
    geng_worker = geng_user;
    geng_worker.uc_stack.ss_sp = geng_stack;
    geng_worker.uc_stack.ss_size = sizeof(geng_stack);
    geng_worker.uc_link = &geng_user;

    makecontext(&geng_worker, (void (*) (void)) GENG_MAIN, 3, geng_argc, p_argv[0], p_argv[1]);
    swapcontext(&geng_user, &geng_worker);
    free(geng_argv);
}

bool
geng_iterator_next(__attribute__((unused)) struct geng_iterator *iter, graph *g)
{
    if (iter_done == 1) return false;
    else
    {
        memcpy(g, cur, sizeof(set) * gn);
        swapcontext(&geng_user, &geng_worker);
        if (iter_done == 0 && generate_done == 1)
            iter_done = 1;
        return true;
    }
}

// TODO: only on macos
#pragma GCC diagnostic pop
