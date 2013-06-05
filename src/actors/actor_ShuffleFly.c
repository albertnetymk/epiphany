#include "actors/actor_ShuffleFly.h"


//(

#define sz  self->sz; 
//)
int D0;
int D1;

 inline void  a0  (actor_ShuffleFly *self) { 
    int a = ConsumeToken(self->X0, 1);
    int b = ConsumeToken(self->X1, 1);
  D0 = a;
  D1 = b;
  }
 inline void  a1  (actor_ShuffleFly *self) { 
    int d2 = ConsumeToken(self->X0, 1);
    int d3 = ConsumeToken(self->X1, 1);
  SendToken(self->Y0, (D0 + d2), 1);
  SendToken(self->Y1, (D0 - d2), 1);
  SendToken(self->Y2, (D1 + d3), 1);
  SendToken(self->Y3, (D1 - d3), 1);
  }

int shufflefly_state=0;
static void run(actor_ShuffleFly *self)
{
if (shufflefly_state == 0) 
  if (TestInputPort(self->X1, 1)) 
    if (TestInputPort(self->X0, 1)) 
      { 
        a0(self);
        shufflefly_state = 5;
      } 
    else 
      { 
        shufflefly_state = 1;
      } 
  else 
    { 
      shufflefly_state = 0;
    } 
else if (shufflefly_state == 5) 
  if (TestInputPort(self->X1, 1)) 
    if (TestInputPort(self->X0, 1)) 
      { 
        a1(self);
        shufflefly_state = 0;
      } 
    else 
      { 
        shufflefly_state = 6;
      } 
  else 
    { 
      shufflefly_state = 5;
    } 
else if (shufflefly_state == 1) 
  if (TestInputPort(self->X0, 1)) 
    { 
      a0(self);
      shufflefly_state = 5;
    } 
  else 
    { 
      shufflefly_state = 1;
    } 
else if (shufflefly_state == 6) 
  if (TestInputPort(self->X0, 1)) 
    { 
      a1(self);
      shufflefly_state = 0;
    } 
  else 
    { 
      shufflefly_state = 6;
    } 

}
static bool not_finished(actor_ShuffleFly* self)
{
    return might_has_input(self->X0)
        || might_has_input(self->X1);
}
static void end(actor_ShuffleFly *self)
{
     end_port(self->Y0);
     end_port(self->Y1);
     end_port(self->Y2);
     end_port(self->Y3);
}
void actor_ShuffleFly_init(actor_ShuffleFly *self)
{

/***********

    sz =  self->sz; 
****************/

    port_in_init(self->X0);
    port_in_init(self->X1);
    port_out_init(self->Y0);
    port_out_init(self->Y1);
    port_out_init(self->Y2);
    port_out_init(self->Y3);
    self->run = &run;
    self->not_finished = &not_finished;
    self->end = &end;
}
