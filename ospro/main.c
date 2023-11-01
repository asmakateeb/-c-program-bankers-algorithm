#include<stdio.h>
#include<stdlib.h>

int P, R;
int *avail;
int **maxm;
int **hold;
int **need;

int readFile(const char *fname) {
    FILE *fp = fopen(fname, "r");
    if(fp == NULL) {
        printf("Failed to open file.\n");
        return -1;
    }

    fscanf(fp, "%d", &P);
    fscanf(fp, "%d", &R);

    maxm = (int**)malloc(P*sizeof(int*));
    hold = (int**)malloc(P*sizeof(int*));
    need = (int**)malloc(P*sizeof(int*));

    avail = (int*)malloc(R*sizeof(int));

    for(int i=0; i<P; i++) {
        maxm[i] = (int*)malloc(R*sizeof(int));
        hold[i] = (int*)malloc(R*sizeof(int));
        need[i] = (int*)malloc(R*sizeof(int));
    }

    for(int i=0; i<P; i++)
        for(int j=0; j<R; j++)
            fscanf(fp, "%d", &maxm[i][j]);

    for(int i=0; i<P; i++)
        for(int j=0; j<R; j++)
            fscanf(fp, "%d", &hold[i][j]);

    for(int i=0; i<R; i++)
        fscanf(fp, "%d", &avail[i]);

    fclose(fp);
    return 0;
}

int checkSafe() {
    int work[R];
    int finish[P];

    for(int i=0; i<R; i++)
        work[i] = avail[i];

    for(int i=0; i<P; i++)
        finish[i] = 0;

    int count = 0;
    while(count < P) {
        int found = 0;
        for(int p=0; p<P; p++) {
            if(finish[p] == 0) {
                int j;
                for(j=0; j<R; j++)
                    if(need[p][j] > work[j])
                        break;
                if(j == R) {
                    for(int k=0; k<R; k++)
                        work[k] += hold[p][k];
                    finish[p] = 1;
                    found = 1;
                    count++;
                }
            }
        }
        if(found == 0) {
            printf("System is unsafe \n");
            return 0;
        }
    }

    printf("System is in safe state.\n and the Safe sequence is: ");
    for(int i=0; i<P; i++)
        if(finish[i] == 1)
            printf("%d ", i);
    printf("\n");
    return 1;
}

void printMatrix(int** matrix, int rows, int cols, char* name) {
    printf("\n%s matrix:\n", name);
    for(int i=0; i<rows; i++) {
        for(int j=0; j<cols; j++)
            printf("%d ", matrix[i][j]);
        printf("\n");
    }
}

void printArray(int* arr, int size, char* name) {
    printf("\n%s array:\n", name);
    for(int i=0; i<size; i++)
        printf("%d ", arr[i]);
    printf("\n");
}

int main() {
    char filename[50];
    printf("file name: ");
    scanf("%s", filename);

    if(readFile(filename) != 0)
        return -1;

    for(int i=0; i<P; i++)
        for(int j=0; j<R; j++)
            need[i][j] = maxm[i][j] - hold[i][j];

    printMatrix(maxm, P, R, "MAXIMUM");
    printMatrix(hold, P, R, "ALLOCATION");
    printMatrix(need, P, R, "NEEDS");
    printArray(avail, R, "AVAILABLE");

    checkSafe();

    int pid;
    int *req = (int*)malloc(R*sizeof(int));
    printf("Enter process ID and resource request array: ");
    scanf("%d", &pid);
    for(int i=0; i<R; i++)
        scanf("%d", &req[i]);

    for(int i=0; i<R; i++) {
        if(req[i] > need[pid][i]) {
            printf("Error.\n");
            return -1;
        }
    }

    for(int i=0; i<R; i++) {
        if(req[i] > avail[i]) {
            printf("Error: Process must wait. Not enough resources.\n");
            return -1;
        }
    }

    for(int i=0; i<R; i++) {
        avail[i] -= req[i];
        hold[pid][i] += req[i];
        need[pid][i] -= req[i];
    }
    //update all matrices
    if(checkSafe()) {
        printf("Request is granted.\n");
        printMatrix(maxm, P, R, "MAXIMUM");
        printMatrix(hold, P, R, "ALLOCATION");
        printMatrix(need, P, R, "NEEDS");
        printArray(avail, R, "AVAILABLE");
    }
    else {
        printf("Request is denied.\n");
        for(int i=0; i<R; i++) {
            avail[i] += req[i];
            hold[pid][i] -= req[i];
            need[pid][i] += req[i];
        }
    }

    free(req);
    for(int i=0; i<P; i++) {
        free(maxm[i]);
        free(hold[i]);
        free(need[i]);
    }
    free(maxm);
    free(hold);
    free(need);
    free(avail);
    return 0;
}

