#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
int row, col;
int row1, col1;

int **arr; //2-d ARRAy
int **arr1;
int **res;

struct arg
{
    int row;
    int col;
};

void *elementByElement(void *arg)
{
    struct arg *args = arg;

    int thread_sum = 0; 
    int i = 0;
    
    for (i = 0; i < row1; i++)
    {

        thread_sum = thread_sum + (arr[args->row][i] * arr1[i][args->col]);
    }
    //store the result to the Multiplication matrix .
    res[args->row][args->col] = thread_sum;
}
void *rowThread(void *arg)
{
    struct arg *args = arg;

    int thread_sum = 0; 
    int i = 0;
    int j = 0;
    
    for (j = 0; j < col1; j++)
    {
        for (i = 0; i < row1; i++)
        {

            thread_sum = thread_sum + (arr[args->row][i] * arr1[i][(args->col) + j]);
        }
       
        res[args->row][(args->col) + j] = thread_sum;
        thread_sum = 0;
    }
}

int main()
{
    char *fname = (char *)malloc(20 + 1);
    printf("Enter File Name:\n");
    scanf("%s", fname);
    FILE *fp;
    // int flag = 0;  // main flag for determine which line in file
    // int flag1 = 0; // key flag in matrix aprsing
    if ((fp = fopen(fname, "r")) == NULL)
    {
        printf("Cannot open the file");
        exit(1);
    }
    fscanf(fp, "%d%d", &row, &col);
    arr = malloc(sizeof(int *) * row); //2-d ARRAy
    for (int i = 0; i < row; i++)
    {
        arr[i] = malloc(sizeof(int) * col);
    }
    for (int i = 0; i < row; i++)
        for (int j = 0; j < col; j++)
            fscanf(fp, "%d", &arr[i][j]);
    fscanf(fp, "%d%d", &row1, &col1);
    arr1 = malloc(sizeof(int *) * row1); //2-d ARRAy
    for (int i = 0; i < row1; i++)
    {
        arr1[i] = malloc(sizeof(int) * col1);
    }
    for (int i = 0; i < row1; i++)
        for (int j = 0; j < col1; j++)
            fscanf(fp, "%d", &arr1[i][j]);

    free(fname);
    fclose(fp);

    int i = 0;
    int n = 0;
    int j = 0;
    res = malloc(sizeof(int *) * row);
    for (i = 0; i < row; i++)
    {
        res[i] = malloc(sizeof(int) * col1);
    }

    // for (i = 0; i < row; i++)
    // {
    //     for (j = 0; j < col; j++)
    //     {
    //         printf("%d\t", arr[i][j]);
    //     }
    //     printf("\n");
    // }

    int threads_num = row * col1;

    pthread_t *threads; 
    struct arg *args;
    args = (struct arg *)malloc((threads_num) * sizeof(struct arg)); // here error
    threads = (pthread_t *)malloc((threads_num) * sizeof(pthread_t));
    clock_t start2, end2;
    double cpu_time_used2;
    int current_thread = 0;
    int row_current, col_current;
    start2 = clock();

    for (row_current = 0; row_current < row; row_current++)
    {
        for (col_current = 0; col_current < col1; col_current++)
        {
            //record the current row of first matrix and column of second matrix so as to pass these data to thread function.
            args[current_thread].row = row_current;
            args[current_thread].col = col_current;

            pthread_create(&threads[current_thread], NULL, elementByElement, (void *)&args[current_thread]);

            current_thread++; //increase the index of threads and args.
        }
    }

    for (i = 0; i < threads_num; i++)
    {
        pthread_join(threads[i], NULL);
    }

    end2 = clock();
    cpu_time_used2 = ((double)(end2 - start2)) / CLOCKS_PER_SEC;

    for (i = 0; i < row; i++)
    {
        for (j = 0; j < col1; j++)
        {
            printf("%d\t", res[i][j]);
        }
        printf("\n");
    }

    FILE *fptr;

    // opening file in writing mode
    fptr = fopen("output.txt", "a");

    // exiting program
    if (fptr == NULL)
    {
        printf("Error!");
        exit(1);
    }

    for (i = 0; i < row; i++)
    {
        for (j = 0; j < col1; j++)
        {
            fprintf(fptr, "%d ", res[i][j]);
        }
        fprintf(fptr, "\n");
    }
    fprintf(fptr, "End1     %f\n", cpu_time_used2);
    //setting array to zero for next computation

    for (i = 0; i < row; i++)
    {
        for (j = 0; j < col1; j++)
        {
            res[i][j] = 0;
        }
    }

    int threads_num1 = row;

    pthread_t *threads1; //to generate an array of thread.
    struct arg *args1;
    args1 = (struct arg *)malloc((threads_num1) * sizeof(struct arg)); // here error
    threads1 = (pthread_t *)malloc((threads_num1) * sizeof(pthread_t));
    clock_t start1, end1;
    double cpu_time_used1;
    int current_thread1 = 0;
    int row_current1, col_current1;
    start1 = clock();

    for (row_current1 = 0; row_current1 < row; row_current1++)
    {

        args1[current_thread1].row = row_current1;
        args1[current_thread1].col = 0;

        pthread_create(&threads1[current_thread1], NULL, rowThread, (void *)&args1[current_thread1]);

        current_thread1++; //increase the index of threads and args.
    }

    for (i = 0; i < threads_num1; i++)
    {
        pthread_join(threads1[i], NULL);
    }

    end1 = clock();
    cpu_time_used1 = ((double)(end1 - start1)) / CLOCKS_PER_SEC;

    free(args1);
    free(threads1);

    for (i = 0; i < row; i++)
    {
        for (j = 0; j < col1; j++)
        {
            fprintf(fptr, "%d ", res[i][j]);
        }
        fprintf(fptr, "\n");
    }
    fprintf(fptr, "End2     %f\n", cpu_time_used1);
    fclose(fptr);

    for (i = 0; i < row; i++)
    {
        free(arr[i]);
    }
    for (i = 0; i < row1; i++)
    {
        free(arr1[i]);
    }

    for (i = 0; i < row; i++)
    {
        free(res[i]);
    }
    free(res);
    free(arr1);
    free(arr);
    free(args);
    free(threads);

    return 0;
}
