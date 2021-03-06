#include "actors/actor_Clip_source_sink.h"


//(

#define isz  self->isz 
#define osz  self->osz 
//)
int count;
bool sflag;
int int_state_vars_Clip=1;
void initStateVarsClip() {
   count = ( - 1) ;

}
bool read_signed_guard (actor_Clip_source_sink *self) { 
      int s = network_read(&Mailbox.n_source[1]);

  return (count < 0);  }
bool limit_max_guard (actor_Clip_source_sink *self) { 
      int i = ReadToken(self->I, 1);

  return (i > 255);  }
bool limit_zero_guard (actor_Clip_source_sink *self) { 
      int i = ReadToken(self->I, 1);

  return (! sflag) && (i < 0);  }
bool limit_min_guard (actor_Clip_source_sink *self) { 
      int i = ReadToken(self->I, 1);

  return (i < ( - 255) );  }

 inline void  read_signed  (actor_Clip_source_sink *self) { 
    int s = network_consume(&Mailbox.n_source[1]);
  sflag = s;
  count = 63;
  }
 inline void  limit_max  (actor_Clip_source_sink *self) { 
    int i = ConsumeToken(self->I, 1);
  count = (count - 1);
  network_write(&Mailbox.n_sink[0], 255);
  }
 inline void  limit_zero  (actor_Clip_source_sink *self) { 
    int i = ConsumeToken(self->I, 1);
  count = (count - 1);
  network_write(&Mailbox.n_sink[0], 0);
  }
 inline void  limit_min  (actor_Clip_source_sink *self) { 
    int i = ConsumeToken(self->I, 1);
  count = (count - 1);
  network_write(&Mailbox.n_sink[0], ( - 255) );
  }
 inline void  limit_none  (actor_Clip_source_sink *self) { 
    int i = ConsumeToken(self->I, 1);
  count = (count - 1);
  network_write(&Mailbox.n_sink[0], i);
  }

int idct_clip_state=0;
static void run(actor_Clip_source_sink *self)
{
    if(int_state_vars_Clip==1){
       int_state_vars_Clip=0;
       initStateVarsClip();
    }
if (idct_clip_state == 0) 
  if (network_not_finished(&Mailbox.n_source[1])) 
    { 
      if (read_signed_guard(self)) 
        { 
          read_signed(self);
          idct_clip_state = 0;
        } 
      else 
        if (TestInputPort(self->I, 1)) 
          { 
            if (limit_max_guard(self)) 
              { 
                limit_max(self);
                idct_clip_state = 0;
              } 
            else 
              { 
                if (limit_zero_guard(self)) 
                  { 
                    if (limit_zero_guard(self)) 
                      { 
                        limit_zero(self);
                        idct_clip_state = 0;
                      } 
                    else 
                      { 
                        if (limit_min_guard(self)) 
                          { 
                            limit_min(self);
                            idct_clip_state = 0;
                          } 
                        else 
                          { 
                            limit_none(self);
                            idct_clip_state = 0;
                          } 
                      } 
                  } 
                else 
                  { 
                    if (limit_min_guard(self)) 
                      { 
                        limit_min(self);
                        idct_clip_state = 0;
                      } 
                    else 
                      { 
                        limit_none(self);
                        idct_clip_state = 0;
                      } 
                  } 
              } 
          } 
        else 
          { 
            idct_clip_state = 4;
          } 
    } 
  else 
    if (TestInputPort(self->I, 1)) 
      { 
        if (limit_max_guard(self)) 
          { 
            limit_max(self);
            idct_clip_state = 0;
          } 
        else 
          { 
            if (limit_zero_guard(self)) 
              { 
                if (limit_zero_guard(self)) 
                  { 
                    limit_zero(self);
                    idct_clip_state = 0;
                  } 
                else 
                  { 
                    if (limit_min_guard(self)) 
                      { 
                        limit_min(self);
                        idct_clip_state = 0;
                      } 
                    else 
                      { 
                        limit_none(self);
                        idct_clip_state = 0;
                      } 
                  } 
              } 
            else 
              { 
                if (limit_min_guard(self)) 
                  { 
                    limit_min(self);
                    idct_clip_state = 0;
                  } 
                else 
                  { 
                    limit_none(self);
                    idct_clip_state = 0;
                  } 
              } 
          } 
      } 
    else 
      { 
        idct_clip_state = 0;
      } 
else if (idct_clip_state == 4) 
  if (TestInputPort(self->I, 1)) 
    { 
      if (limit_max_guard(self)) 
        { 
          limit_max(self);
          idct_clip_state = 0;
        } 
      else 
        { 
          if (limit_zero_guard(self)) 
            { 
              if (limit_zero_guard(self)) 
                { 
                  limit_zero(self);
                  idct_clip_state = 0;
                } 
              else 
                { 
                  if (limit_min_guard(self)) 
                    { 
                      limit_min(self);
                      idct_clip_state = 0;
                    } 
                  else 
                    { 
                      limit_none(self);
                      idct_clip_state = 0;
                    } 
                } 
            } 
          else 
            { 
              if (limit_min_guard(self)) 
                { 
                  limit_min(self);
                  idct_clip_state = 0;
                } 
              else 
                { 
                  limit_none(self);
                  idct_clip_state = 0;
                } 
            } 
        } 
    } 
  else 
    { 
      idct_clip_state = 4;
    } 

}
static bool not_finished(actor_Clip_source_sink* self)
{
    return might_has_input(self->I)
        || network_not_finished(&Mailbox.n_source[1]);
}
static void end(actor_Clip_source_sink *self)
{
     end_port(self->O);
}
void actor_Clip_source_sink_init(actor_Clip_source_sink *self)
{

/***********

    isz =  self->isz; 
    osz =  self->osz; 
****************/

    port_in_init(self->I);
    port_in_init(self->SIGNED);
    port_out_init(self->O);
    self->run = &run;
    self->not_finished = &not_finished;
    self->end = &end;
}
