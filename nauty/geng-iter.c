#include "gtools.h"
#include "geng.h"
#include <ucontext.h>
#include <stdint.h>
#include <stdbool.h>

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
OUTPROC(__attribute__((unused)) FILE *outfile,
        graph *g, int n, struct geng_iterator *iter)
{
    memcpy(&iter->batch[iter->batch_size * n], g, sizeof(set) * n);
    iter->graph_size = n;
    iter->batch_size++;

    if (iter->batch_size == iter->batch_capacity)
        swapcontext(&iter->geng_worker, &iter->geng_user);
}

// TODO: probably don't need to name this function with macro
// TODO: move to geng.h maybe?
extern int
GENG_MAIN(int argc, char *argv[]);

// geng_iterator_init gives ownership of iterator memory to caller
void
geng_iterator_create(struct geng_iterator **iterator_ptr,
                     size_t graph_size,
                     size_t batch_capacity)
{
    *iterator_ptr = malloc(sizeof(struct geng_iterator));
    
    struct geng_iterator *iterator = *iterator_ptr;
    iterator->batch = malloc(sizeof(set) * graph_size * batch_capacity);
    iterator->batch_capacity = batch_capacity;
    iterator->batch_size = 0;

    iterator->iteration_done = false;

    // TODO: add support for more arguments
    int geng_argc = 3;
    char **geng_argv;
    geng_argv = malloc((geng_argc + 1) * sizeof(char *));
    geng_argv[0] = "geng";
    geng_argv[1] = "-q";
    char n_str[20];
    snprintf(n_str, sizeof(n_str), "%zu", graph_size);
    geng_argv[2] = n_str;
    geng_argv[3] = NULL;

    // TODO: make macro
    uint32_t p_argv[2];
    p_argv[0] = (uint32_t) (((size_t) &geng_argv) & ((1llu << 32) - 1llu));
    p_argv[1] = ((size_t) &geng_argv) >> 32;

    uint32_t p_iter[2];
    p_iter[0] = (uint32_t) (((size_t) &iterator) & ((1llu << 32) - 1llu));
    p_iter[1] = ((size_t) &iterator) >> 32;

    getcontext(&iterator->geng_user);
    iterator->geng_worker = iterator->geng_user;
    iterator->geng_worker.uc_stack.ss_sp = iterator->geng_stack;
    iterator->geng_worker.uc_stack.ss_size = sizeof(iterator->geng_stack);
    iterator->geng_worker.uc_link = &iterator->geng_user;

    makecontext(
        &iterator->geng_worker, (void (*) (void)) GENG_MAIN,
        5, geng_argc, p_argv[0], p_argv[1], p_iter[0], p_iter[1]
    );
    swapcontext(&iterator->geng_user, &iterator->geng_worker);
    free(geng_argv);
}

bool
geng_iterator_next(struct geng_iterator *iter, set *g)
{
    static int i = 0;

    if (i == iter->batch_size && iter->generation_done)
        iter->iteration_done = true;

    if (iter->iteration_done) return false;
    else
    {
        if (i == iter->batch_size && !iter->generation_done)
        {
            iter->batch_size = 0;
            i = 0;
            swapcontext(&iter->geng_user, &iter->geng_worker);
        }

        memcpy(
            g,
            &iter->batch[i * iter->graph_size],
            sizeof(set) * iter->graph_size
        );
        i++;
        return true;
    }
}

void
geng_iterator_destroy(struct geng_iterator *iter)
{
    free(iter->batch);
}

// TODO: only on macos
#pragma GCC diagnostic pop
