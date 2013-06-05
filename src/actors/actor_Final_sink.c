#include "actors/actor_Final_sink.h"


//(

#define isz  self->isz 
#define osz  self->osz 
//)

 inline void  final  (actor_Final_sink *self) { 
    int a = ConsumeToken(self->X0, 1);
    int b = ConsumeToken(self->X1, 1);
    int c = ConsumeToken(self->X2, 1);
    int d = ConsumeToken(self->X3, 1);
  network_write(&Mailbox.n_sink[0], ((a + c) >> 8));
  network_write(&Mailbox.n_sink[1], ((a - c) >> 8));
  network_write(&Mailbox.n_sink[2], ((b + d) >> 8));
  network_write(&Mailbox.n_sink[3], ((b - d) >> 8));
  }

int final_state=0;
static void run(actor_Final_sink *self)
{
if (final_state == 0) 
  if (TestInputPort(self->X1, 1)) 
    if (TestInputPort(self->X0, 1)) 
      if (TestInputPort(self->X2, 1)) 
        if (TestInputPort(self->X3, 1)) 
          { 
            final(self);
            final_state = 0;
          } 
        else 
          { 
            final_state = 5;
          } 
      else 
        { 
          final_state = 3;
        } 
    else 
      { 
        final_state = 1;
      } 
  else 
    { 
      final_state = 0;
    } 
else if (final_state == 1) 
  if (TestInputPort(self->X0, 1)) 
    if (TestInputPort(self->X2, 1)) 
      if (TestInputPort(self->X3, 1)) 
        { 
          final(self);
          final_state = 0;
        } 
      else 
        { 
          final_state = 5;
        } 
    else 
      { 
        final_state = 3;
      } 
  else 
    { 
      final_state = 1;
    } 
else if (final_state == 3) 
  if (TestInputPort(self->X2, 1)) 
    if (TestInputPort(self->X3, 1)) 
      { 
        final(self);
        final_state = 0;
      } 
    else 
      { 
        final_state = 5;
      } 
  else 
    { 
      final_state = 3;
    } 
else if (final_state == 5) 
  if (TestInputPort(self->X3, 1)) 
    { 
      final(self);
      final_state = 0;
    } 
  else 
    { 
      final_state = 5;
    } 

}
static bool not_finished(actor_Final_sink* self)
{
    return might_has_input(self->X0)
        || might_has_input(self->X1)
        || might_has_input(self->X2)
        || might_has_input(self->X3);
}
static void end(actor_Final_sink *self)
{
     end_port(self->Y0);
     end_port(self->Y1);
     end_port(self->Y2);
     end_port(self->Y3);
}
void actor_Final_sink_init(actor_Final_sink *self)
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
