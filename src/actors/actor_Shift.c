#include "actors/actor_Shift.h"


//(

#define isz  self->isz 
#define csz  self->csz; 
//)

 inline void  Shift  (actor_Shift *self) { 
    int a = ConsumeToken(self->X0, 1);
    int b = ConsumeToken(self->X1, 1);
    int c = ConsumeToken(self->X2, 1);
    int d = ConsumeToken(self->X3, 1);
  SendToken(self->Y0, (a >> 6), 1);
  SendToken(self->Y1, (b >> 6), 1);
  SendToken(self->Y2, (c >> 6), 1);
  SendToken(self->Y3, (d >> 6), 1);
  }

int idct_shift_state=0;
static void run(actor_Shift *self)
{
if (idct_shift_state == 0) 
  if (TestInputPort(self->X0, 1)) 
    if (TestInputPort(self->X1, 1)) 
      if (TestInputPort(self->X2, 1)) 
        if (TestInputPort(self->X3, 1)) 
          { 
            Shift(self);
            idct_shift_state = 0;
          } 
        else 
          { 
            wait();
            idct_shift_state = 5;
          } 
      else 
        { 
          wait();
          idct_shift_state = 3;
        } 
    else 
      { 
        wait();
        idct_shift_state = 1;
      } 
  else 
    { 
      wait();
      idct_shift_state = 0;
    } 
else if (idct_shift_state == 1) 
  if (TestInputPort(self->X1, 1)) 
    if (TestInputPort(self->X2, 1)) 
      if (TestInputPort(self->X3, 1)) 
        { 
          Shift(self);
          idct_shift_state = 0;
        } 
      else 
        { 
          wait();
          idct_shift_state = 5;
        } 
    else 
      { 
        wait();
        idct_shift_state = 3;
      } 
  else 
    { 
      wait();
      idct_shift_state = 1;
    } 
else if (idct_shift_state == 3) 
  if (TestInputPort(self->X2, 1)) 
    if (TestInputPort(self->X3, 1)) 
      { 
        Shift(self);
        idct_shift_state = 0;
      } 
    else 
      { 
        wait();
        idct_shift_state = 5;
      } 
  else 
    { 
      wait();
      idct_shift_state = 3;
    } 
else if (idct_shift_state == 5) 
  if (TestInputPort(self->X3, 1)) 
    { 
      Shift(self);
      idct_shift_state = 0;
    } 
  else 
    { 
      wait();
      idct_shift_state = 5;
    } 

}
static bool not_finished(actor_Shift* self)
{
    return might_has_input(self->X0)
        || might_has_input(self->X1)
        || might_has_input(self->X2)
        || might_has_input(self->X3);
}
static void end(actor_Shift *self)
{
     end_port(self->Y0);
     end_port(self->Y1);
     end_port(self->Y2);
     end_port(self->Y3);
}
void actor_Shift_init(actor_Shift *self)
{

/***********

    isz =  self->isz; 
    csz =  self->csz; 
****************/

    port_in_init(self->X0);
    port_in_init(self->X1);
    port_in_init(self->X2);
    port_in_init(self->X3);
    port_out_init(self->Y0);
    port_out_init(self->Y1);
    port_out_init(self->Y2);
    port_out_init(self->Y3);
    self->run = &run;
    self->not_finished = &not_finished;
    self->end = &end;
}
