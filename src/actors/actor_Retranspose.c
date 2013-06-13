#include "actors/actor_Retranspose.h"


//(

#define isz  self->isz 
#define osz  self->osz 
//)
// Print list, set , collection .... variable declarations
 
int mem[2][8][8];
int rcount = 0;
int ccount = 0;
int select = 0;
int int_state_vars_Retranspose=1;
void initStateVarsRetranspose() {
    int i, j, k;
   for( k = 1- 1; k<=8 - 1; k ++)
for( j = 1- 1; j<=8 - 1; j ++)
for( i = 1- 1; i<=2 - 1; i ++)
mem[i][j][k] = 0;
}
bool RetransOne_guard (actor_Retranspose *self) { 
      int a = ReadToken(self->X0, 1);

      int b = ReadToken(self->X1, 1);

      int c = ReadToken(self->X2, 1);

      int d = ReadToken(self->X3, 1);

  int row = (rcount >> 3);

  int quad = ((rcount >> 2) & 1);

  return (rcount < 64);  }
bool RetranceTwo_guard (actor_Retranspose *self) { 
  int col = ((64 - ccount) >> 3);

  int row = ((64 - ccount) & 7);

  int i = (~(select&1) | (~select & ~1));

  return (ccount > 0);  }
bool RetranceThree_guard (actor_Retranspose *self) { 
  return (ccount == 0);  }

 inline void  RetransOne  (actor_Retranspose *self) { 
    int a = ConsumeToken(self->X0, 1);
    int b = ConsumeToken(self->X1, 1);
    int c = ConsumeToken(self->X2, 1);
    int d = ConsumeToken(self->X3, 1);
  int row = (rcount >> 3);

  int quad = ((rcount >> 2) & 1);

  if (quad == 0) 
    { 
      mem[select] [row] [0] = a;
      mem[select] [row] [7] = b;
      mem[select] [row] [3] = c;
      mem[select] [row] [4] = d;
    } 
  else 
    { 
      mem[select] [row] [1] = a;
      mem[select] [row] [6] = b;
      mem[select] [row] [2] = c;
      mem[select] [row] [5] = d;
    } 
  rcount = (rcount + 4);
  }
 inline void  RetranceTwo  (actor_Retranspose *self) { 
  int col = ((64 - ccount) >> 3);

  int row = ((64 - ccount) & 7);

  int i = (~(select&1) | (~select & ~1));

  ccount = (ccount - 1);
  SendToken(self->Y, mem[i][row][col], 1);
  }
 inline void  RetranceThree  (actor_Retranspose *self) { 
  select = (~(select&1) | (~select & ~1));
  ccount = 64;
  rcount = 0;
  }

int idct_retrans_state=0;
static void run(actor_Retranspose *self)
{
    if(int_state_vars_Retranspose==1){
       int_state_vars_Retranspose=0;
       initStateVarsRetranspose();
    }
if (idct_retrans_state == 0) 
  if (TestInputPort(self->X3, 1)) 
    if (TestInputPort(self->X2, 1)) 
      if (TestInputPort(self->X1, 1)) 
        if (TestInputPort(self->X0, 1)) 
          { 
            if (RetranceThree_guard(self)) 
              { 
                RetranceThree(self);
                idct_retrans_state = 0;
              } 
            else 
              { 
                if (RetranceTwo_guard(self)) 
                  { 
                    RetranceTwo(self);
                    idct_retrans_state = 0;
                  } 
                else 
                  { 
                    if (RetransOne_guard(self)) 
                      { 
                        RetransOne(self);
                        idct_retrans_state = 0;
                      } 
                    else 
                      dead();
                  } 
              } 
          } 
        else 
          { 
            if (RetranceTwo_guard(self)) 
              { 
                RetranceTwo(self);
                idct_retrans_state = 0;
              } 
            else 
              { 
                if (RetranceThree_guard(self)) 
                  { 
                    RetranceThree(self);
                    idct_retrans_state = 0;
                  } 
                else 
                  { 
                    wait();
                    idct_retrans_state = 38;
                  } 
              } 
          } 
      else 
        { 
          if (RetranceThree_guard(self)) 
            { 
              RetranceThree(self);
              idct_retrans_state = 0;
            } 
          else 
            { 
              if (RetranceTwo_guard(self)) 
                { 
                  RetranceTwo(self);
                  idct_retrans_state = 0;
                } 
              else 
                { 
                  wait();
                  idct_retrans_state = 33;
                } 
            } 
        } 
    else 
      { 
        if (RetranceThree_guard(self)) 
          { 
            RetranceThree(self);
            idct_retrans_state = 0;
          } 
        else 
          { 
            if (RetranceTwo_guard(self)) 
              { 
                RetranceTwo(self);
                idct_retrans_state = 0;
              } 
            else 
              { 
                wait();
                idct_retrans_state = 26;
              } 
          } 
      } 
  else 
    { 
      if (RetranceTwo_guard(self)) 
        { 
          RetranceTwo(self);
          idct_retrans_state = 0;
        } 
      else 
        { 
          if (RetranceThree_guard(self)) 
            { 
              RetranceThree(self);
              idct_retrans_state = 0;
            } 
          else 
            { 
              wait();
              idct_retrans_state = 19;
            } 
        } 
    } 
else if (idct_retrans_state == 19) 
  if (TestInputPort(self->X1, 1)) 
    if (TestInputPort(self->X3, 1)) 
      if (TestInputPort(self->X0, 1)) 
        if (TestInputPort(self->X2, 1)) 
          { 
            if (RetransOne_guard(self)) 
              { 
                RetransOne(self);
                idct_retrans_state = 0;
              } 
            else 
              dead();
          } 
        else 
          { 
            wait();
            idct_retrans_state = 41;
          } 
      else 
        { 
          wait();
          idct_retrans_state = 34;
        } 
    else 
      { 
        wait();
        idct_retrans_state = 27;
      } 
  else 
    { 
      wait();
      idct_retrans_state = 19;
    } 
else if (idct_retrans_state == 26) 
  if (TestInputPort(self->X2, 1)) 
    if (TestInputPort(self->X0, 1)) 
      if (TestInputPort(self->X1, 1)) 
        { 
          if (RetransOne_guard(self)) 
            { 
              RetransOne(self);
              idct_retrans_state = 0;
            } 
          else 
            dead();
        } 
      else 
        { 
          wait();
          idct_retrans_state = 39;
        } 
    else 
      { 
        wait();
        idct_retrans_state = 33;
      } 
  else 
    { 
      wait();
      idct_retrans_state = 26;
    } 
else if (idct_retrans_state == 33) 
  if (TestInputPort(self->X0, 1)) 
    if (TestInputPort(self->X1, 1)) 
      { 
        if (RetransOne_guard(self)) 
          { 
            RetransOne(self);
            idct_retrans_state = 0;
          } 
        else 
          dead();
      } 
    else 
      { 
        wait();
        idct_retrans_state = 39;
      } 
  else 
    { 
      wait();
      idct_retrans_state = 33;
    } 
else if (idct_retrans_state == 38) 
  if (TestInputPort(self->X0, 1)) 
    { 
      if (RetransOne_guard(self)) 
        { 
          RetransOne(self);
          idct_retrans_state = 0;
        } 
      else 
        dead();
    } 
  else 
    { 
      wait();
      idct_retrans_state = 38;
    } 
else if (idct_retrans_state == 27) 
  if (TestInputPort(self->X3, 1)) 
    if (TestInputPort(self->X0, 1)) 
      if (TestInputPort(self->X2, 1)) 
        { 
          if (RetransOne_guard(self)) 
            { 
              RetransOne(self);
              idct_retrans_state = 0;
            } 
          else 
            dead();
        } 
      else 
        { 
          wait();
          idct_retrans_state = 41;
        } 
    else 
      { 
        wait();
        idct_retrans_state = 34;
      } 
  else 
    { 
      wait();
      idct_retrans_state = 27;
    } 
else if (idct_retrans_state == 34) 
  if (TestInputPort(self->X0, 1)) 
    if (TestInputPort(self->X2, 1)) 
      { 
        if (RetransOne_guard(self)) 
          { 
            RetransOne(self);
            idct_retrans_state = 0;
          } 
        else 
          dead();
      } 
    else 
      { 
        wait();
        idct_retrans_state = 41;
      } 
  else 
    { 
      wait();
      idct_retrans_state = 34;
    } 
else if (idct_retrans_state == 39) 
  if (TestInputPort(self->X1, 1)) 
    { 
      if (RetransOne_guard(self)) 
        { 
          RetransOne(self);
          idct_retrans_state = 0;
        } 
      else 
        dead();
    } 
  else 
    { 
      wait();
      idct_retrans_state = 39;
    } 
else if (idct_retrans_state == 41) 
  if (TestInputPort(self->X2, 1)) 
    { 
      if (RetransOne_guard(self)) 
        { 
          RetransOne(self);
          idct_retrans_state = 0;
        } 
      else 
        dead();
    } 
  else 
    { 
      wait();
      idct_retrans_state = 41;
    } 

}
static bool not_finished(actor_Retranspose* self)
{
    return might_has_input(self->X0)
        || might_has_input(self->X1)
        || might_has_input(self->X2)
        || might_has_input(self->X3);
}
static void end(actor_Retranspose *self)
{
     end_port(self->Y);
}
void actor_Retranspose_init(actor_Retranspose *self)
{

/***********

    isz =  self->isz; 
    osz =  self->osz; 
****************/

    port_in_init(self->X0);
    port_in_init(self->X1);
    port_in_init(self->X2);
    port_in_init(self->X3);
    port_out_init(self->Y);
    self->run = &run;
    self->not_finished = &not_finished;
    self->end = &end;
}
