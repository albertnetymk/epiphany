#include "actors/actor_Scale_source.h"


//(

#define isz  self->isz 
#define osz  self->osz 
#define csz  self->csz; 
//)
// Print list, set , collection .... variable declarations
 
int W0 [4];
// Print list, set , collection .... variable declarations
 
int W1 [4];
int ww0;
int ww1 = 2048;
int local_index = 0;
int int_state_vars_Scale=1;
void initStateVarsScale() {
     W0[0] = 2048;
  W0[1] = 2676;
  W0[2] = 2841;
  W0[3] = 1609;
     W1[0] = 2048;
  W1[1] = 1108;
  W1[2] = 565;
  W1[3] = 2408;
   ww0 = W0[0];

}

 inline void  scale  (actor_Scale_source *self) { 
    int a = network_consume(&Mailbox.n_source[0]);
    int b = network_consume(&Mailbox.n_source[1]);
  int w0 = ww0;

  int w1 = ww1;

  local_index = ((local_index + 1) & 3);
  ww0 = W0[local_index];
  ww1 = W1[local_index];
  SendToken(self->Y0, (a * w0), 1);
  SendToken(self->Y1, (a * w1), 1);
  SendToken(self->Y2, (b * w0), 1);
  SendToken(self->Y3, (b * w1), 1);
  }

int scale_state=0;
static void run(actor_Scale_source *self)
{
    if(int_state_vars_Scale==1){
       int_state_vars_Scale=0;
       initStateVarsScale();
    }
if (scale_state == 0) 
  if (network_has_input(&Mailbox.n_source[1])) 
    if (network_has_input(&Mailbox.n_source[0])) 
      { 
        scale(self);
        scale_state = 0;
      } 
    else 
      { 
        scale_state = 1;
      } 
  else 
    { 
      scale_state = 0;
    } 
else if (scale_state == 1) 
  if (network_has_input(&Mailbox.n_source[0])) 
    { 
      scale(self);
      scale_state = 0;
    } 
  else 
    { 
      scale_state = 1;
    } 

}
static bool not_finished(actor_Scale_source* self)
{
    return network_not_finished(&Mailbox.n_source[0])
        || network_not_finished(&Mailbox.n_source[1]);
}
static void end(actor_Scale_source *self)
{
     end_port(self->Y0);
     end_port(self->Y1);
     end_port(self->Y2);
     end_port(self->Y3);
}
void actor_Scale_source_init(actor_Scale_source *self)
{

/***********

    isz =  self->isz; 
    osz =  self->osz; 
    csz =  self->csz; 
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
