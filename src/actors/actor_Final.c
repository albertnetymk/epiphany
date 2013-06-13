#include "actors/actor_Final.h"


//(

#define isz  self->isz 
#define osz  self->osz 
//)

 inline void  final  (actor_Final *self) { 
    int a = ConsumeToken(self->X0, 1);
    int b = ConsumeToken(self->X1, 1);
    int c = ConsumeToken(self->X2, 1);
    int d = ConsumeToken(self->X3, 1);
  SendToken(self->Y0, ((a + c) >> 8), 1);
  SendToken(self->Y1, ((a - c) >> 8), 1);
  SendToken(self->Y2, ((b + d) >> 8), 1);
  SendToken(self->Y3, ((b - d) >> 8), 1);
  }

int idctCol_final_state=0;
static void run(actor_Final *self)
{
if (idctCol_final_state == 0) 
  if (TestInputPort(self->X1, 1)) 
    if (TestInputPort(self->X3, 1)) 
      if (TestInputPort(self->X0, 1)) 
        if (TestInputPort(self->X2, 1)) 
          { 
            final(self);
            idctCol_final_state = 0;
          } 
        else 
          { 
            wait();
            idctCol_final_state = 5;
          } 
      else 
        { 
          wait();
          idctCol_final_state = 3;
        } 
    else 
      { 
        wait();
        idctCol_final_state = 1;
      } 
  else 
    { 
      wait();
      idctCol_final_state = 0;
    } 
else if (idctCol_final_state == 1) 
  if (TestInputPort(self->X3, 1)) 
    if (TestInputPort(self->X0, 1)) 
      if (TestInputPort(self->X2, 1)) 
        { 
          final(self);
          idctCol_final_state = 0;
        } 
      else 
        { 
          wait();
          idctCol_final_state = 5;
        } 
    else 
      { 
        wait();
        idctCol_final_state = 3;
      } 
  else 
    { 
      wait();
      idctCol_final_state = 1;
    } 
else if (idctCol_final_state == 3) 
  if (TestInputPort(self->X0, 1)) 
    if (TestInputPort(self->X2, 1)) 
      { 
        final(self);
        idctCol_final_state = 0;
      } 
    else 
      { 
        wait();
        idctCol_final_state = 5;
      } 
  else 
    { 
      wait();
      idctCol_final_state = 3;
    } 
else if (idctCol_final_state == 5) 
  if (TestInputPort(self->X2, 1)) 
    { 
      final(self);
      idctCol_final_state = 0;
    } 
  else 
    { 
      wait();
      idctCol_final_state = 5;
    } 

}
static bool not_finished(actor_Final* self)
{
    return might_has_input(self->X0)
        || might_has_input(self->X1)
        || might_has_input(self->X2)
        || might_has_input(self->X3);
}
static void end(actor_Final *self)
{
     end_port(self->Y0);
     end_port(self->Y1);
     end_port(self->Y2);
     end_port(self->Y3);
}
void actor_Final_init(actor_Final *self)
{

/***********

    isz =  self->isz; 
    osz =  self->osz; 
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
