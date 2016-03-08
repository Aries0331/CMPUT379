#include "factory.h"

/** @brief Each assembler creates a product of unique color
 *         and each product is numbered sequentially.
 *         After creating a product, the assembler will place
 *         it onto the assembly line (the bounded-buffer array).
 *         If the assembly line is full, the assembler must wait until a slot becomes available.
 *
 *  @param A pointer but usually NULL
 *
 *  @return void.
 */

void * assembler (void * ptr)
{
    int i;
    int cn;
    
    /* assign a single color of product to each assembler */
    pthread_mutex_lock(&mutex);
    cn = color_num;
    color_num = (color_num+1) % MAXCOLORS;
    pthread_mutex_unlock(&mutex);
    
    for(i=0;i<c;i++) {
        
        pthread_mutex_lock(&mutex);

        /* check if the queue is full */
        while(count == l) {
            pthread_cond_wait(&cond_a, &mutex);
        }
        
        buffer[in].color = cn;
        buffer[in].index = i+1;
        in = (in+1)%l;
        count++;
        numitems--;
        
        pthread_cond_broadcast(&cond_p); // in case of more than one thread is in a blocking wait state
        pthread_mutex_unlock(&mutex);
    }
    
    pthread_exit(0);
}

