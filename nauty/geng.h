#include "gtools.h"
#include <ucontext.h>
#include <stdbool.h>

struct geng_iterator
{
    ucontext_t geng_worker, geng_user;
    char geng_stack[1 << 20];
    int graph_size;
    bool generation_done;
    bool iteration_done;
    int batch_size;
    int batch_capacity;
    graph **batch;

    // TODO: remove
    graph *cur;
};
