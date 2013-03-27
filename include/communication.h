#ifndef _COMMUNICATION_H
#define _COMMUNICATION_H

typedef unsigned int uint;
typedef unsigned int uchar;

typedef struct port_in_struct {
    volatile uchar read_index;
    volatile uchar write_index;
    int array[10];
} port_in;

typedef struct port_out_struct {
    struct port_in_struct *dest;
} port_out;

void epiphany_write(port_out *p, int v);
int epiphany_read(port_in *p);
void connect(port_out *out, port_in *in);

#endif /* end of include guard: _COMMUNICATION_H */
