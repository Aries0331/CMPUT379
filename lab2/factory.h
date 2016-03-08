#ifndef factory_h
#define factory_h

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#define MAXCOLORS 147

struct item {
    int color; // variable "color" indicates a color among 147 colors
    int index; // variable "index" indicates the exact number of the color, like color1,color2...
};
extern struct item * buffer; // bounded-buffer

extern pthread_mutex_t mutex;
extern pthread_cond_t cond_a; // conditional variable for assembler
extern pthread_cond_t cond_p; // conditional variable for packer

extern int color_num;

extern int a; //the number of assembler threads
extern int p; //the number of packer threads
extern int l; //the size of the assembly line
extern int n; //the number of products per box
extern int c; //the number of products each assembler will produce

extern int count; // count the total item in the circular-queue
extern int in;
extern int out;
extern int numitems; // keep track of the number of items have put in queue, total is a*c

void * packer (void * ptr);
void * assembler (void * ptr);

#endif /* factory_h */
