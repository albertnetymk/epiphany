#include "actors/actor_Shuffle.h"


//(

#define sz  self->sz; 
//)
int x4;
int x5;
int x6h;
int x7h;
int x6l;
int x7l;

 inline void  a0  (actor_Shuffle *self) { 
    int a = ConsumeToken(self->X0, 1);
    int b = ConsumeToken(self->X1, 1);
    int c = ConsumeToken(self->X2, 1);
    int d = ConsumeToken(self->X3, 1);
  x4 = c;
  x5 = d;
  SendToken(self->Y0, a, 1);
  SendToken(self->Y1, b, 1);
  }
 inline void  a1  (actor_Shuffle *self) { 
    int x2 = ConsumeToken(self->X0, 1);
    int a = ConsumeToken(self->X1, 1);
    int x3 = ConsumeToken(self->X2, 1);
    int b = ConsumeToken(self->X3, 1);
  int ah = (a >> 8);

  int bh = (b >> 8);

  int al = (a & 255);

  int bl = (b & 255);

  x6h = (181 * (ah + bh));
  x7h = (181 * (ah - bh));
  x6l = (181 * (al + bl));
  x7l = (181 * (al - bl));
  SendToken(self->Y2, x2, 1);
  SendToken(self->Y3, x3, 1);
  }
 inline void  a2  (actor_Shuffle *self) { 
  SendToken(self->Y0, x4, 1);
  SendToken(self->Y1, x5, 1);
  SendToken(self->Y2, (x6h + (x6l >> 8)), 1);
  SendToken(self->Y3, (x7h + (x7l >> 8)), 1);
  }

int idctCol_shuffle_state=0;
static void run(actor_Shuffle *self)
{
if (idctCol_shuffle_state == 0) 
  if (TestInputPort(self->X2, 1)) 
    if (TestInputPort(self->X1, 1)) 
      if (TestInputPort(self->X3, 1)) 
        if (TestInputPort(self->X0, 1)) 
          { 
            a0(self);
            idctCol_shuffle_state = 9;
          } 
        else 
          { 
            wait();
            idctCol_shuffle_state = 5;
          } 
      else 
        { 
          wait();
          idctCol_shuffle_state = 3;
        } 
    else 
      { 
        wait();
        idctCol_shuffle_state = 1;
      } 
  else 
    { 
      wait();
      idctCol_shuffle_state = 0;
    } 
else if (idctCol_shuffle_state == 1) 
  if (TestInputPort(self->X1, 1)) 
    if (TestInputPort(self->X3, 1)) 
      if (TestInputPort(self->X0, 1)) 
        { 
          a0(self);
          idctCol_shuffle_state = 9;
        } 
      else 
        { 
          wait();
          idctCol_shuffle_state = 5;
        } 
    else 
      { 
        wait();
        idctCol_shuffle_state = 3;
      } 
  else 
    { 
      wait();
      idctCol_shuffle_state = 1;
    } 
else if (idctCol_shuffle_state == 3) 
  if (TestInputPort(self->X3, 1)) 
    if (TestInputPort(self->X0, 1)) 
      { 
        a0(self);
        idctCol_shuffle_state = 9;
      } 
    else 
      { 
        wait();
        idctCol_shuffle_state = 5;
      } 
  else 
    { 
      wait();
      idctCol_shuffle_state = 3;
    } 
else if (idctCol_shuffle_state == 9) 
  if (TestInputPort(self->X0, 1)) 
    if (TestInputPort(self->X1, 1)) 
      if (TestInputPort(self->X3, 1)) 
        if (TestInputPort(self->X2, 1)) 
          { 
            a1(self);
            idctCol_shuffle_state = 18;
          } 
        else 
          { 
            wait();
            idctCol_shuffle_state = 14;
          } 
      else 
        { 
          wait();
          idctCol_shuffle_state = 12;
        } 
    else 
      { 
        wait();
        idctCol_shuffle_state = 10;
      } 
  else 
    { 
      wait();
      idctCol_shuffle_state = 9;
    } 
else if (idctCol_shuffle_state == 5) 
  if (TestInputPort(self->X0, 1)) 
    { 
      a0(self);
      idctCol_shuffle_state = 9;
    } 
  else 
    { 
      wait();
      idctCol_shuffle_state = 5;
    } 
else if (idctCol_shuffle_state == 10) 
  if (TestInputPort(self->X1, 1)) 
    if (TestInputPort(self->X3, 1)) 
      if (TestInputPort(self->X2, 1)) 
        { 
          a1(self);
          idctCol_shuffle_state = 18;
        } 
      else 
        { 
          wait();
          idctCol_shuffle_state = 14;
        } 
    else 
      { 
        wait();
        idctCol_shuffle_state = 12;
      } 
  else 
    { 
      wait();
      idctCol_shuffle_state = 10;
    } 
else if (idctCol_shuffle_state == 12) 
  if (TestInputPort(self->X3, 1)) 
    if (TestInputPort(self->X2, 1)) 
      { 
        a1(self);
        idctCol_shuffle_state = 18;
      } 
    else 
      { 
        wait();
        idctCol_shuffle_state = 14;
      } 
  else 
    { 
      wait();
      idctCol_shuffle_state = 12;
    } 
else if (idctCol_shuffle_state == 18) 
  { 
    a2(self);
    idctCol_shuffle_state = 0;
  } 
else if (idctCol_shuffle_state == 14) 
  if (TestInputPort(self->X2, 1)) 
    { 
      a1(self);
      idctCol_shuffle_state = 18;
    } 
  else 
    { 
      wait();
      idctCol_shuffle_state = 14;
    } 

}
static bool not_finished(actor_Shuffle* self)
{
    return might_has_input(self->X0)
        || might_has_input(self->X1)
        || might_has_input(self->X2)
        || might_has_input(self->X3);
}
static void end(actor_Shuffle *self)
{
     end_port(self->Y0);
     end_port(self->Y1);
     end_port(self->Y2);
     end_port(self->Y3);
}
void actor_Shuffle_init(actor_Shuffle *self)
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
    port_out_init(self->Y2);
    port_out_init(self->Y3);
    self->run = &run;
    self->not_finished = &not_finished;
    self->end = &end;
}
