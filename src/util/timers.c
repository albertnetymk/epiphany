#include <e_ctimers.h>
#include <stdbool.h>
#include "util/types.h"

bool initialized[2] = { false, false };

uint get_clock()
{
    return E_CTIMER_MAX - e_ctimer_get(E_CTIMER_0);
}

static void init_timer(uchar id)
{
    initialized[id] = true;
    switch(id) {
        case 0:
            e_ctimer_set(E_CTIMER_0, E_CTIMER_CLK, E_CTIMER_MAX);
            break;
        case 1:
            e_ctimer_set(E_CTIMER_1, E_CTIMER_CLK, E_CTIMER_MAX);
            break;
    }
}

void timer_resume(uchar id)
{
    if (!initialized[id]) {
        init_timer(id);
    }
    switch(id) {
        case 0:
            e_ctimer_start(E_CTIMER_0, E_CTIMER_CLK);
            break;
        case 1:
            e_ctimer_start(E_CTIMER_1, E_CTIMER_CLK);
            break;
    }
}

void timer_pause(uchar id)
{
    switch(id) {
        case 0:
            e_ctimer_stop(E_CTIMER_0);
            break;
        case 1:
            e_ctimer_stop(E_CTIMER_1);
            break;
    }
}

uint get_time()
{
    return E_CTIMER_MAX - e_ctimer_get(E_CTIMER_1);
}
