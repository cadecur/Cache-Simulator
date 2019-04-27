// Authors: Cade Curry (ccurry), Ishaan Gandhi (igandhi)


/* 
 * trans.c - Matrix transpose B = A^T
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 */ 
#include <stdio.h>
#include "cachelab.h"


int is_transpose(int M, int N, int A[N][M], int B[M][N]);

/* 
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded. 
 */
char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{
    int vblock, hblock, i, j, tmp;
    if (M == 32 && N == 32) {
        for ( vblock = 0; vblock < 4; vblock++) { // run 4 blocks across the matrix vertically, each with 8 ints
            for ( hblock = 0; hblock < 4; hblock++) { // run 4 blocks across the matrix horizontally, each with 8 ints
                for ( i = 0; i < 8; i++) {
                    for (j = 0; j < 8; j++) {
                        tmp = A[vblock*8+i][hblock*8+j];
                        B[hblock*8+j][vblock*8+i] = tmp;
                    }
                }
            }
        }
    }
    if (M == 61 && N == 67){
        for ( vblock = 0; vblock < 8; vblock++) { // run 4 blocks across the matrix vertically, each with 8 ints
            for ( hblock = 0; hblock < 7; hblock++) { // run 4 blocks across the matrix horizontally, each with 8 ints
                for ( i = 0; i < 8; i++) {
                    for (j = 0; j < 8; j++) {
                        tmp = A[vblock*8+i][hblock*8+j];
                        B[hblock*8+j][vblock*8+i] = tmp;
                    }
                }
            }
        }
        for ( vblock = 0; vblock < 8; vblock++ ){
            for ( i = 0; i < 8; i++){
                for (j = 0; j < 5; j++){
                    tmp = A[vblock*8+i][56+j];
                    B[56+j][vblock*8+i] = tmp;
                }
            }
        }
        for ( hblock = 0; hblock < 7; hblock++ ){
            for ( i = 0 ; i < 3; i++){
                for ( j = 0; j < 8; j++){
                    tmp = A[64+i][hblock*8+j];
                    B[hblock*8+j][64+i] = tmp;
                }
            }
        }
    
        for ( i = 61; i < 67; i++ ){
            for ( j = 56; j < 61; j++){
                tmp = A[i][j];
                B[j][i] = tmp;
            }
        }
    }
}

/* 
 * You can define additional transpose functions below. We've defined
 * a simple one below to help you get started. 
 */ 

/* 
 * trans - A simple baseline transpose function, not optimized for the cache.
 */
char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, tmp;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            tmp = A[i][j];
            B[j][i] = tmp;

        }
    }    

}

/*
 * registerFunctions - This function registers your transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     transpose strategies.
 */
void registerFunctions()
{
    /* Register your solution function */
    registerTransFunction(transpose_submit, transpose_submit_desc); 

    /* Register any additional transpose functions */
    registerTransFunction(trans, trans_desc); 

}

/* 
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; ++j) {
            if (A[i][j] != B[j][i]) {
                return 0;
            }
        }
    }
    return 1;
}

