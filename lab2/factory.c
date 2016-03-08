// cmput379 Lab2
// NAME: JINZHU LI
// Student ID: 1461696

#include "factory.h"

struct item * buffer;
pthread_mutex_t mutex;
pthread_cond_t cond_a;
pthread_cond_t cond_p;
int color_num,count,in,out,numitems;
int a,p,l,n,c;

int main(int argc, char * argv[])
{
    int i,j;
    
    if(argc != 6) {
        printf("usage: ./Factory a p l n c\n");
    }
    a = atoi(argv[1]);
    p = atoi(argv[2]);
    l = atoi(argv[3]);
    n = atoi(argv[4]);
    c = atoi(argv[5]);
    
    buffer = (struct item *)malloc(sizeof(struct item) * l); // initialize the circular queue
    numitems = a*c;
    color_num = 0;
    count = 0;
    in = 0;
    out = 0;
    
    pthread_t tid_a[a];
    pthread_t tid_p[p];
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond_a,NULL);
    pthread_cond_init(&cond_p,NULL);
    
    /* create assembler and packer threads according to the number of input */
    for(i=0;i<a;i++) {
        int err_a = pthread_create(&tid_a[i], NULL, assembler, NULL);
        if(err_a != 0)
            perror("Error creating assembler threads.\n");
    }
    for(j=0;j<p;j++) {
        int err_p = pthread_create(&tid_p[j], NULL, packer, NULL);
        if(err_p != 0)
            perror("Error creating packer threads.\n");
    }
    
    /* wait all the threads to finish their jobs */
    for(i=0;i<a;i++)
        pthread_join(tid_a[i], NULL);
    for(j=0;j<p;j++)
        pthread_join(tid_p[j], NULL);
    
    free(buffer);
    return 0;
}

