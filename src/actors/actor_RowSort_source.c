#include "actors/actor_RowSort_source.h"


//(

#define sz  self->sz; 
//)
int x0;
int x1;
int x2;
int x3;
int x5;

 inline void  a0  (actor_RowSort_source *self) { 
    int a = network_consume(&Mailbox.n_source[0]);
  x0 = a;
  }
 inline void  a1  (actor_RowSort_source *self) { 
    int a = network_consume(&Mailbox.n_source[0]);
  x1 = a;
  }
 inline void  a2  (actor_RowSort_source *self) { 
    int a = network_consume(&Mailbox.n_source[0]);
  x2 = a;
  }
 inline void  a3  (actor_RowSort_source *self) { 
    int a = network_consume(&Mailbox.n_source[0]);
  x3 = a;
  }
 inline void  a4  (actor_RowSort_source *self) { 
    int a = network_consume(&Mailbox.n_source[0]);
  SendToken(self->Y0, x0, 1);
  SendToken(self->Y1, a, 1);
  }
 inline void  a5  (actor_RowSort_source *self) { 
    int a = network_consume(&Mailbox.n_source[0]);
  x5 = a;
  }
 inline void  a6  (actor_RowSort_source *self) { 
    int a = network_consume(&Mailbox.n_source[0]);
  SendToken(self->Y0, x2, 1);
  SendToken(self->Y1, a, 1);
  }
 inline void  a7  (actor_RowSort_source *self) { 
    int a = network_consume(&Mailbox.n_source[0]);
  SendToken(self->Y0, x1, 1);
  SendToken(self->Y1, a, 1);
  }
 inline void  a8  (actor_RowSort_source *self) { 
    int a = network_consume(&Mailbox.n_source[0]);
  x0 = a;
  SendToken(self->Y0, x5, 1);
  SendToken(self->Y1, x3, 1);
  }
 inline void  a9  (actor_RowSort_source *self) { 
  SendToken(self->Y0, x5, 1);
  SendToken(self->Y1, x3, 1);
  }

int idct_rowsort_state=0;
static void run(actor_RowSort_source *self)
{
if (idct_rowsort_state == 0) 
  if (network_not_finished(&Mailbox.n_source[0])) 
    { 
      a0(self);
      idct_rowsort_state = 3;
    } 
  else 
    { 
      idct_rowsort_state = 0;
    } 
else if (idct_rowsort_state == 3) 
  if (network_not_finished(&Mailbox.n_source[0])) 
    { 
      a1(self);
      idct_rowsort_state = 6;
    } 
  else 
    { 
      idct_rowsort_state = 3;
    } 
else if (idct_rowsort_state == 6) 
  if (network_not_finished(&Mailbox.n_source[0])) 
    { 
      a2(self);
      idct_rowsort_state = 9;
    } 
  else 
    { 
      idct_rowsort_state = 6;
    } 
else if (idct_rowsort_state == 9) 
  if (network_not_finished(&Mailbox.n_source[0])) 
    { 
      a3(self);
      idct_rowsort_state = 12;
    } 
  else 
    { 
      idct_rowsort_state = 9;
    } 
else if (idct_rowsort_state == 12) 
  if (network_not_finished(&Mailbox.n_source[0])) 
    { 
      a4(self);
      idct_rowsort_state = 15;
    } 
  else 
    { 
      idct_rowsort_state = 12;
    } 
else if (idct_rowsort_state == 15) 
  if (network_not_finished(&Mailbox.n_source[0])) 
    { 
      a5(self);
      idct_rowsort_state = 18;
    } 
  else 
    { 
      idct_rowsort_state = 15;
    } 
else if (idct_rowsort_state == 18) 
  if (network_not_finished(&Mailbox.n_source[0])) 
    { 
      a6(self);
      idct_rowsort_state = 21;
    } 
  else 
    { 
      idct_rowsort_state = 18;
    } 
else if (idct_rowsort_state == 21) 
  if (network_not_finished(&Mailbox.n_source[0])) 
    { 
      a7(self);
      idct_rowsort_state = 24;
    } 
  else 
    { 
      idct_rowsort_state = 21;
    } 
else if (idct_rowsort_state == 24) 
  if (network_not_finished(&Mailbox.n_source[0])) 
    { 
      a8(self);
      idct_rowsort_state = 3;
    } 
  else 
    { 
      a9(self);
      idct_rowsort_state = 0;
    } 

}
static bool not_finished(actor_RowSort_source* self)
{
    return network_not_finished(&Mailbox.n_source[0]);
}
static void end(actor_RowSort_source *self)
{
     end_port(self->Y0);
     end_port(self->Y1);
}
void actor_RowSort_source_init(actor_RowSort_source *self)
{

/***********

    sz =  self->sz; 
****************/

    port_in_init(self->ROW);
    port_out_init(self->Y0);
    port_out_init(self->Y1);
    self->run = &run;
    self->not_finished = &not_finished;
    self->end = &end;
}
