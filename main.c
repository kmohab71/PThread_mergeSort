//
//  main.c
//  mergeSortPthreads
//
//  Created by Khaled Mohab on 11/7/18.
//  Copyright © 2018 Khaled Mohab. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
int *array;
int length;
struct arg
{
    int start;
    int end;
};
void initArgument(struct arg* arg,int start,int end)
{
    (arg)->start=start;
    (arg)->end=end;
}
void printArray(int A[], int end)
{
    int i;
    for (i=0; i <= end; i++)
        printf("%d ", A[i]);
    printf("\n");
}
void swap(int a ,int b)
{
    int temp;
    temp = array[a];
    array[a]=array[b];
    array[b]=temp;
}
void  *mergeSort(void * arg)
{
    struct arg *arguments=(struct arg*)arg;
    struct arg argArr[2];
    if (arguments->start < arguments->end )
    {
        int middle=(arguments->end+arguments->start)/2;
        argArr[0].start=arguments->start;
        argArr[0].end=middle;
        argArr[1].start=middle+1;
        argArr[1].end=arguments->end;
        pthread_t tids[2];
        pthread_attr_t attr;
        pthread_attr_init(&attr);
        pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
        for (int i=0; i<2; i++)
        {
            pthread_create(&tids[i], &attr, mergeSort, (void*)&argArr[i]);
        }
        for (int i=0; i<2; i++)
        {
            pthread_join(tids[i], NULL);
        }
        int length=(argArr[1].end - argArr[0].start +1);
        int *back=(int*)malloc(length * sizeof(int));
        int i = 0, j = argArr[0].start, k = middle+1,p;
        while (j <= middle && k <= argArr[1].end)
        {
            if (array[j] <= array[k])
            {
                back[i++] = array[j++];
            }
            else
            {
                back[i++] = array[k++];
            }
        }
        if (j==middle+1)
        {
            while (k <= argArr[1].end)
            {
                back[i++] = array[k++];
            }
        }else
        {
            while (j <= middle)
            {
                back[i++] = array[j++];
            }
        }
        p=0;
        for (i=argArr[0].start; i<=argArr[1].end; i++) {
            array[i]=back[p++];
        }
    }
    pthread_exit(0);
}

int main(int argc, const char * argv[]) {
    struct arg arguments;
    length=200000;
    array = (int*) malloc(sizeof(int)*length);
    for (int i=0; i<length; i++) {
        array[i]=rand()%10000000000;
    }
    arguments.start=0;
    arguments.end=length;
    pthread_t id;
    pthread_create(&id,NULL,mergeSort,(void *)&arguments);
    pthread_join(id, NULL);
    printArray(array,arguments.end);
    return 0;
}
