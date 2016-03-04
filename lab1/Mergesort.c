#include "Mergesort.h"
/* create shared memory mechansim */
int shmem_id, *shmem_array, shmem_size;

int main(int argc, char *argv[])
{
    FILE * input;
    int i, j, data, length;
    int * array;
    
    length = 0;
    
    /* allocate a new memory space for the array for the first time */
    array = malloc(sizeof(int));
    
    /* Read the file from the command line */
    input = fopen(argv[1], "r");
    
    /* check if the file is successfully read */
    if(input == NULL) {
        printf("Could not open the file.");
        exit(EXIT_FAILURE);
    }
    
    printf("Sorting file: %s\n", argv[1]);
    
    /* put all the numbers read from the input file into the array */
    while(fscanf(input, "%d", &data) != -1) {
        ++length;
        /* realloc the memory space to fit the unlimited amount of numbers */
        array = realloc(array, length * sizeof(int));
        array[length-1] = data;
    }
    
    printf("%d elements read\n\n", length);
    printf("Input Numbers:\n");
    
    for(j=0;j<length;j++) {
        printf("%d ",array[j]);
    }
    printf("\n\n");
    
    shmem_size = length * sizeof(int);
    
    /* allocate a shared memory segment */
    shmem_id = shmget(IPC_PRIVATE, shmem_size, IPC_CREAT | 0666);
    if(shmem_id == -1) {
        perror("shemget error");
        exit(1);
    }
    
    /* attach the shared memory segment */
    shmem_array = shmat(shmem_id, NULL, 0);
    if(shmem_array == (void *)-1) {
        perror("shemat error");
        exit(1);
    }
    
    /* write the original array to be sorted into the shared memory */
    for(i=0;i<length;i++) {
        shmem_array[i] = array[i];
    }
    
    /* mergesort algorithm */
    MERGE_SORT(shmem_array,0,length-1);
    
    /* Print out the sorted array */
    printf("Sorted Numbers:\n");
    for(int i=0;i<length;i++) {
        printf("%d ",shmem_array[i]);
    }
    printf("\n");
    
    fclose(input);
    free(array);
    
    /* detach the shared memory segment */
    if(shmdt(shmem_array) == -1) {
        perror("shmdt");
        exit(1);
    }
    
    /* remove the shared memory segment */
    if(shmctl(shmem_id, IPC_RMID, NULL) == -1) {
        perror("shmctl");
        exit(1);
    }

    return 0;
}

void MERGE(int array[], int begin, int mid, int end)
{
    int * temp;
    
    int i, j;
    i = begin;
    j = mid + 1;
    
    int size;
    size = end + 1;
    temp = malloc(size * sizeof(int));
    
    /* Copy of the input arrays */
    memcpy(temp, array, size * sizeof(int));
    
    int k = begin;
    
    /* Compare each element in the two halves */
    while(i<=mid && j<=end) {
        if(temp[i] <= temp[j]) {
            array[k] = temp[i];
            k++;
            i++;
        }
        else {
            array[k] = temp[j];
            k++;
            j++;
        }
    }
    
    /* Append the remaining elements to the array */
    while(i<=mid) {
        array[k] = temp[i];
        k++;
        i++;
    }
    while(j<=end) {
        array[k] = temp[j];
        k++;
        j++;
    }
    
    /* free the memory in case of memory leak */
    free(temp);
}

void MERGE_SORT(int array[], int begin, int end)
{
    int mid;
    int lpid, rpid;
    
    if(begin < end) {
        mid = (begin + end) / 2;
        
        /* create the first child */
        lpid = fork();
        if(lpid < 0) {
            perror("fork");
            exit(1);
        }
        else if(lpid == 0) {
            MERGE_SORT(array,begin,mid);
            shmdt(shmem_array);
            exit(0);
        }
        else {
            waitpid(lpid,NULL,0);
            /* create the second child */
            rpid = fork();
            if(rpid < 0) {
                perror("fork");
                exit(1);
            }
            else if(rpid == 0) {
                MERGE_SORT(array,mid+1,end);
                shmdt(shmem_array);
                exit(0);
            }
        }
        
        /* The parent process has to wait for both children to finish their work */
        waitpid(rpid,NULL,0);
        
        MERGE(array,begin,mid,end);
    }

}
