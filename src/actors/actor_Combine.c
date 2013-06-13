#include "actors/actor_Combine.h"


//(

#define isz  self->isz 
#define osz  self->osz 
#define row  self->row 
//)
int count = 0;

 inline void  combine  (actor_Combine *self) { 
    int a = ConsumeToken(self->X0, 1);
    int b = ConsumeToken(self->X1, 1);
    int c = ConsumeToken(self->X2, 1);
    int d = ConsumeToken(self->X3, 1);
  bool s = (count == 0);

  int o = (row ? (s ? 128 : 0) : (s ? 65536 : 4));

  int y0 = ((a + d) + o);

  int y1 = ((b - c) + o);

  int y0out = (row ? y0 : (y0 >> 3));

  int y1out = (row ? y1 : (y1 >> 3));

  count = ((count + 1) & 3);
  SendToken(self->Y0, y0out, 1);
  SendToken(self->Y1, y1out, 1);
  }

int idctCol_combine_state=0;
static void run(actor_Combine *self)
{
if (idctCol_combine_state == 0) 
  if (TestInputPort(self->X1, 1)) 
    if (TestInputPort(self->X2, 1)) 
      if (TestInputPort(self->X3, 1)) 
        if (TestInputPort(self->X0, 1)) 
          { 
            combine(self);
            idctCol_combine_state = 0;
          } 
        else 
          { 
            idctCol_combine_state = 5;
          } 
      else 
        { 
          idctCol_combine_state = 3;
        } 
    else 
      { 
        idctCol_combine_state = 1;
      } 
  else 
    { 
      idctCol_combine_state = 0;
    } 
else if (idctCol_combine_state == 1) 
  if (TestInputPort(self->X2, 1)) 
    if (TestInputPort(self->X3, 1)) 
      if (TestInputPort(self->X0, 1)) 
        { 
          combine(self);
          idctCol_combine_state = 0;
        } 
      else 
        { 
          idctCol_combine_state = 5;
        } 
    else 
      { 
        idctCol_combine_state = 3;
      } 
  else 
    { 
      idctCol_combine_state = 1;
    } 
else if (idctCol_combine_state == 3) 
  if (TestInputPort(self->X3, 1)) 
    if (TestInputPort(self->X0, 1)) 
      { 
        combine(self);
        idctCol_combine_state = 0;
      } 
    else 
      { 
        idctCol_combine_state = 5;
      } 
  else 
    { 
      idctCol_combine_state = 3;
    } 
else if (idctCol_combine_state == 5) 
  if (TestInputPort(self->X0, 1)) 
    { 
      combine(self);
      idctCol_combine_state = 0;
    } 
  else 
    { 
      idctCol_combine_state = 5;
    } 

}
static bool not_finished(actor_Combine* self)
{
    return might_has_input(self->X0)
        || might_has_input(self->X1)
        || might_has_input(self->X2)
        || might_has_input(self->X3);
}
static void end(actor_Combine *self)
{
     end_port(self->Y0);
     end_port(self->Y1);
}
void actor_Combine_init(actor_Combine *self)
{

/***********

    isz =  self->isz; 
    osz =  self->osz; 
    row =  self->row; 
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
