#include "actors/actor_Transpose.h"

//(

#define sz  self->sz;
//)
// Print list, set , collection .... variable declarations

int mem[2][8][8];
int rcount = 0;
int ccount = 0;
int select = 0;
int int_state_vars_Transpose = 1;
void initStateVarsTranspose()
{
    int i, j, k;
    for (k = 1 - 1; k <= 8 - 1; k++) {
        for (j = 1 - 1; j <= 8 - 1; j++) {
            for (i = 1 - 1; i <= 2 - 1; i++) {
                mem[i][j][k] = 0;
            }
        }
    }
}
bool transposeOne_guard(actor_Transpose *self)
{
    int a = ReadToken(self->X0, 1);

    int b = ReadToken(self->X1, 1);

    int c = ReadToken(self->X2, 1);

    int d = ReadToken(self->X3, 1);

    int row = (rcount >> 3);

    int quad = ((rcount >> 2) & 1);

    return(rcount < 64);
}
bool transposeTwo_guard(actor_Transpose *self)
{
    int a;

    int b;

    int col = ((64 - ccount) >> 3);

    int pair = (((64 - ccount) >> 1) & 3);

    // int i = (~(select & 1) | (~select & ~1));
    int i = select ^ 1;

    return(ccount > 0);
}
bool transposeThre_guard(actor_Transpose *self)
{
    return(ccount == 0 && rcount == 64);
}

inline void  transposeOne(actor_Transpose *self)
{
    int a = ConsumeToken(self->X0, 1);
    int b = ConsumeToken(self->X1, 1);
    int c = ConsumeToken(self->X2, 1);
    int d = ConsumeToken(self->X3, 1);
    int row = (rcount >> 3);

    int quad = ((rcount >> 2) & 1);

    if (quad == 0) {
        mem[select] [row] [0] = a;
        mem[select] [row] [7] = b;
        mem[select] [row] [3] = c;
        mem[select] [row] [4] = d;
    }else    {
        mem[select] [row] [1] = a;
        mem[select] [row] [6] = b;
        mem[select] [row] [2] = c;
        mem[select] [row] [5] = d;
    }
    rcount = (rcount + 4);
}
inline void  transposeTwo(actor_Transpose *self)
{
    int a;

    int b;

    int col = ((64 - ccount) >> 3);

    int pair = (((64 - ccount) >> 1) & 3);

    // int i = (~(select & 1) | (~select & ~1));
    int i = select ^ 1;

    a = (pair == 0) ? 0 : ((pair == 1) ? 2 : ((pair == 2) ? 1 : 5));
    b = (pair == 0) ? 4 : ((pair == 1) ? 6 : ((pair == 2) ? 7 : 3));
    ccount = (ccount - 2);
    SendToken(self->Y0, mem[i][a][col], 1);
    SendToken(self->Y1, mem[i][b][col], 1);
}
inline void  transposeThre(actor_Transpose *self)
{
    // select = (~(select & 1) | (~select & ~1));
    select = select ^ 1;
    ccount = 64;
    rcount = 0;
}

int idct_trans_state = 0;
static void run(actor_Transpose *self)
{
    if (int_state_vars_Transpose == 1) {
        int_state_vars_Transpose = 0;
        initStateVarsTranspose();
    }
    if (idct_trans_state == 0) {
        if (transposeThre_guard(self)) {
            transposeThre(self);
            idct_trans_state = 0;
        }else
            if (TestInputPort(self->X0, 1)) {
                if (TestInputPort(self->X1, 1)) {
                    if (TestInputPort(self->X3, 1)) {
                        if (transposeTwo_guard(self)) {
                            transposeTwo(self);
                            idct_trans_state = 0;
                        }else
                            if (TestInputPort(self->X2, 1)) {
                                if (transposeOne_guard(self)) {
                                    transposeOne(self);
                                    idct_trans_state = 0;
                                }else    {
                                    dead();
                                }
                            }else    {
                                idct_trans_state = 15;
                            }
                    }else    {
                        if (transposeTwo_guard(self)) {
                            transposeTwo(self);
                            idct_trans_state = 0;
                        }else    {
                            idct_trans_state = 23;
                        }
                    }
                } else{
                    if (transposeTwo_guard(self)) {
                        transposeTwo(self);
                        idct_trans_state = 0;
                    }else    {
                        idct_trans_state = 18;
                    }
                }
            } else{
                if (transposeTwo_guard(self)) {
                    transposeTwo(self);
                    idct_trans_state = 0;
                }else    {
                    idct_trans_state = 13;
                }
            }
    }else if (idct_trans_state == 13) {
        if (TestInputPort(self->X2, 1)) {
            if (TestInputPort(self->X1, 1)) {
                if (TestInputPort(self->X3, 1)) {
                    if (TestInputPort(self->X0, 1)) {
                        if (transposeOne_guard(self)) {
                            transposeOne(self);
                            idct_trans_state = 0;
                        }else    {
                            dead();
                        }
                    }else    {
                        idct_trans_state = 28;
                    }
                } else{
                    idct_trans_state = 24;
                }
            } else{
                idct_trans_state = 19;
            }
        } else{
            idct_trans_state = 13;
        }
    }else if (idct_trans_state == 18) {
        if (TestInputPort(self->X1, 1)) {
            if (TestInputPort(self->X3, 1)) {
                if (TestInputPort(self->X2, 1)) {
                    if (transposeOne_guard(self)) {
                        transposeOne(self);
                        idct_trans_state = 0;
                    }else    {
                        dead();
                    }
                }else    {
                    idct_trans_state = 15;
                }
            } else{
                idct_trans_state = 23;
            }
        } else{
            idct_trans_state = 18;
        }
    }else if (idct_trans_state == 23) {
        if (TestInputPort(self->X3, 1)) {
            if (TestInputPort(self->X2, 1)) {
                if (transposeOne_guard(self)) {
                    transposeOne(self);
                    idct_trans_state = 0;
                }else    {
                    dead();
                }
            }else    {
                idct_trans_state = 15;
            }
        } else{
            idct_trans_state = 23;
        }
    }else if (idct_trans_state == 15) {
        if (TestInputPort(self->X2, 1)) {
            if (transposeOne_guard(self)) {
                transposeOne(self);
                idct_trans_state = 0;
            }else    {
                dead();
            }
        }else    {
            idct_trans_state = 15;
        }
    }else if (idct_trans_state == 19) {
        if (TestInputPort(self->X1, 1)) {
            if (TestInputPort(self->X3, 1)) {
                if (TestInputPort(self->X0, 1)) {
                    if (transposeOne_guard(self)) {
                        transposeOne(self);
                        idct_trans_state = 0;
                    }else    {
                        dead();
                    }
                }else    {
                    idct_trans_state = 28;
                }
            } else{
                idct_trans_state = 24;
            }
        } else{
            idct_trans_state = 19;
        }
    }else if (idct_trans_state == 24) {
        if (TestInputPort(self->X3, 1)) {
            if (TestInputPort(self->X0, 1)) {
                if (transposeOne_guard(self)) {
                    transposeOne(self);
                    idct_trans_state = 0;
                }else    {
                    dead();
                }
            }else    {
                idct_trans_state = 28;
            }
        } else{
            idct_trans_state = 24;
        }
    }else if (idct_trans_state == 28) {
        if (TestInputPort(self->X0, 1)) {
            if (transposeOne_guard(self)) {
                transposeOne(self);
                idct_trans_state = 0;
            }else    {
                dead();
            }
        }else    {
            idct_trans_state = 28;
        }
    }
}
static bool not_finished(actor_Transpose* self)
{
    return might_has_input(self->X0)
        || might_has_input(self->X1)
        || might_has_input(self->X2)
        || might_has_input(self->X3);
}
static void end(actor_Transpose *self)
{
    end_port(self->Y0);
    end_port(self->Y1);
}
void actor_Transpose_init(actor_Transpose *self)
{
    /***********

      sz =  self->sz;
     ****************/

    port_in_init(self->X0);
    port_in_init(self->X1);
    port_in_init(self->X2);
    port_in_init(self->X3);
    port_out_init(self->Y0);
    port_out_init(self->Y1);
    self->run = &run;
    self->not_finished = &not_finished;
    self->end = &end;
}
