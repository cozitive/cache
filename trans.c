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
void transpose_submit(int M, int N, int A[N][M], int B[M][N]) {
    int r, c, i, j, k = -1;
    int t0, t1, t2, t3, t4;
    if (N == 32 && M == 32) {
        for (r = 0; r < N; r += 8) {
            for (c = 0; c < M; c += 8) {
                for (i = r; i < r + 8; i++) {
                    for (j = c; j < c + 8; j++) {
                        if (i == j) {
                            k = j;
                        } else {
                            B[j][i] = A[i][j];
                        }
                    }
                    if (k >= 0) {
                        B[k][i] = A[i][k];
                        k = -1;
                    }
                }
            }
        }
    } else if (N == 64 && M == 64) {
        for (r = 0; r < N; r += 8) {
            for (c = 0; c < M; c += 8) {
                if (r == c) {   // A, B share same sets (only use 4 sets)
                    B[c + 1][r] = A[r][c + 1];
                    B[c + 2][r] = A[r][c + 2];
                    B[c + 3][r] = A[r][c + 3];
                    t0 = A[r][c];
                    B[c + 1][r + 4] = A[r + 4][c + 1];
                    B[c + 2][r + 4] = A[r + 4][c + 2];
                    B[c + 3][r + 4] = A[r + 4][c + 3];
                    t1 = A[r + 4][c];
                    B[c][r] = t0;

                    B[c][r + 1] = A[r + 1][c];
                    B[c + 2][r + 1] = A[r + 1][c + 2];
                    B[c + 3][r + 1] = A[r + 1][c + 3];
                    t0 = A[r + 1][c + 1];
                    B[c][r + 5] = A[r + 5][c];
                    B[c + 2][r + 5] = A[r + 5][c + 2];
                    B[c + 3][r + 5] = A[r + 5][c + 3];
                    t2 = A[r + 5][c + 1];
                    B[c + 1][r + 1] = t0;

                    B[c][r + 2] = A[r + 2][c];
                    B[c + 1][r + 2] = A[r + 2][c + 1];
                    B[c + 3][r + 2] = A[r + 2][c + 3];
                    t0 = A[r + 2][c + 2];
                    B[c][r + 6] = A[r + 6][c];
                    B[c + 1][r + 6] = A[r + 6][c + 1];
                    B[c + 3][r + 6] = A[r + 6][c + 3];
                    t3 = A[r + 6][c + 2];
                    B[c + 2][r + 2] = t0;

                    B[c][r + 3] = A[r + 3][c];
                    B[c + 1][r + 3] = A[r + 3][c + 1];
                    B[c + 2][r + 3] = A[r + 3][c + 2];
                    t0 = A[r + 3][c + 3];
                    B[c][r + 7] = A[r + 7][c];
                    B[c + 1][r + 7] = A[r + 7][c + 1];
                    B[c + 2][r + 7] = A[r + 7][c + 2];
                    t4 = A[r + 7][c + 3];
                    B[c + 3][r + 3] = t0;

                    B[c][r + 4] = t1;
                    B[c + 1][r + 5] = t2;
                    B[c + 2][r + 6] = t3;
                    B[c + 3][r + 7] = t4;


                    B[c + 5][r] = A[r][c + 5];
                    B[c + 6][r] = A[r][c + 6];
                    B[c + 7][r] = A[r][c + 7];
                    t0 = A[r][c + 4];
                    B[c + 5][r + 4] = A[r + 4][c + 5];
                    B[c + 6][r + 4] = A[r + 4][c + 6];
                    B[c + 7][r + 4] = A[r + 4][c + 7];
                    t1 = A[r + 4][c + 4];
                    B[c + 4][r] = t0;

                    B[c + 4][r + 1] = A[r + 1][c + 4];
                    B[c + 6][r + 1] = A[r + 1][c + 6];
                    B[c + 7][r + 1] = A[r + 1][c + 7];
                    t0 = A[r + 1][c + 5];
                    B[c + 4][r + 5] = A[r + 5][c + 4];
                    B[c + 6][r + 5] = A[r + 5][c + 6];
                    B[c + 7][r + 5] = A[r + 5][c + 7];
                    t2 = A[r + 5][c + 5];
                    B[c + 5][r + 1] = t0;

                    B[c + 4][r + 2] = A[r + 2][c + 4];
                    B[c + 5][r + 2] = A[r + 2][c + 5];
                    B[c + 7][r + 2] = A[r + 2][c + 7];
                    t0 = A[r + 2][c + 6];
                    B[c + 4][r + 6] = A[r + 6][c + 4];
                    B[c + 5][r + 6] = A[r + 6][c + 5];
                    B[c + 7][r + 6] = A[r + 6][c + 7];
                    t3 = A[r + 6][c + 6];
                    B[c + 6][r + 2] = t0;

                    B[c + 4][r + 3] = A[r + 3][c + 4];
                    B[c + 5][r + 3] = A[r + 3][c + 5];
                    B[c + 6][r + 3] = A[r + 3][c + 6];
                    t0 = A[r + 3][c + 7];
                    B[c + 4][r + 7] = A[r + 7][c + 4];
                    B[c + 5][r + 7] = A[r + 7][c + 5];
                    B[c + 6][r + 7] = A[r + 7][c + 6];
                    t4 = A[r + 7][c + 7];
                    B[c + 7][r + 3] = t0;

                    B[c + 4][r + 4] = t1;
                    B[c + 5][r + 5] = t2;
                    B[c + 6][r + 6] = t3;
                    B[c + 7][r + 7] = t4;
                } else {
                    // up, left
                    for (i = r; i < r + 4; i++) {
                        for (j = c; j < c + 4; j++) {
                            if (i % 4 == j % 4) {
                                k = j;
                            } else {
                                B[j][i] = A[i][j];
                            }
                        }
                        if (k >= 0) {
                            B[k][i] = A[i][k];
                            k = -1;
                        }
                    }
                    // down, left
                    for (i = r + 4; i < r + 8; i++) {
                        for (j = c; j < c + 4; j++) {
                            if (i % 4 == j % 4) {
                                k = j;
                            } else {
                                B[j][i] = A[i][j];
                            }
                        }
                        if (k >= 0) {
                            B[k][i] = A[i][k];
                            k = -1;
                        }
                    }
                    // down, right
                    for (i = r + 4; i < r + 8; i++) {
                        for (j = c + 4; j < c + 8; j++) {
                            if (i % 4 == j % 4) {
                                k = j;
                            } else {
                                B[j][i] = A[i][j];
                            }
                        }
                        if (k >= 0) {
                            B[k][i] = A[i][k];
                            k = -1;
                        }
                    }
                    // up, right
                    for (i = r; i < r + 4; i++) {
                        for (j = c + 4; j < c + 8; j++) {
                            if (i % 4 == j % 4) {
                                k = j;
                            } else {
                                B[j][i] = A[i][j];
                            }
                        }
                        if (k >= 0) {
                            B[k][i] = A[i][k];
                            k = -1;
                        }
                    }
                }
            }
        }
    } else if (N == 67 && M == 61) {
        for (r = 0; r < N; r += 8) {
            for (c = 0; c < M; c += 8) {
                for (i = r; i < ((r + 8 < N) ? (r + 8) : N); i++) {
                    for (j = c; j < ((c + 8 < M) ? (c + 8) : M); j++) {
                        B[j][i] = A[i][j];
                    }
                }
            }
        }
    } else {
        for (i = 0; i < N; i++) {
            for (j = 0; j < M; j++) {
                B[j][i] = A[i][j];
            }
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
void registerFunctions() {
    /* Register your solution function */
    registerTransFunction(transpose_submit, transpose_submit_desc);
}

/*
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N]) {
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

