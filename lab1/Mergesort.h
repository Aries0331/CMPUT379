#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/wait.h>
#include <sys/types.h>

/** @brief merge the half sorted parts into the original
 *         array.
 *  @param array[] The array which the merged array
 *         will be written.
 *  @param begin The index of the first element of the
 *         array.
 *  @param mid The index of the center element of the
 *         array.
 *  @param end The index of the last element of the
 *         array.
 *  @return void.
 */
void MERGE(int array[], int begin, int mid, int end);

/** @brief Recursive function to sort an array of integers.
 *         Split a set of input numbers into two halves.
 *         Each half uses mergesort recursively to sort its part.
 *  @param array[] The array to be splited and sorted.
 *  @param begin The index of the first element of the
 *         array.
 *  @param end The index of the last element of the
 *         array.
 *  @return void.
 */
void MERGE_SORT(int array[], int begin, int end);