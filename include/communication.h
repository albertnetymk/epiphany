#ifndef _COMMUNICATION_H
#define _COMMUNICATION_H

typedef struct port_in_struct port_in;
typedef struct port_out_struct port_out;

port_out *port_out_new();
port_in *port_in_new();
void epiphany_write(port_out *p, int v);
int epiphany_read(port_in *p);
void connect(port_out *out, port_in *in);

#endif /* end of include guard: _COMMUNICATION_H */
