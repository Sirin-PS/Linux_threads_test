#include <stdio.h>
#include <iostream>
#include <pthread.h>
#include <errno.h>
#include <unistd.h>

#define N 16

using namespace std;

double X[N][N];
double Y[N][N];

typedef struct {
    int start;
    int end;
    int rank;
} thread_argument;

double func1(double param) {
    if (param > 0)
        return param * param - 1;
    else
        return param * (-1) + 3;
}

void *thread_func1(void *arg) {
    thread_argument tArg = *(thread_argument*)arg;    
    int start_idx = tArg.start;
    int end_idx = tArg.end;
    int num = tArg.rank;

    for (int i = start_idx; i < end_idx; i++) {
        for (int j = 0; j < N; j++) {
            Y[i][j] = func1(X[i][j]);
        }
    }
    // for test:
    //cout << "\nThread with rank " << num << " done!\n";
    pthread_exit(0);
}

void outMatrix(double (*Matr)[N], int n) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            cout << Matr[i][j] << "  ";
        }
        cout << endl;
    }
    cout << endl;
}


int main(int argc, char* argv[]) {
    pthread_t *tids;
    pthread_attr_t attr;
    thread_argument *args;
    int numthreads;    
    
    // initialization
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            X[i][j] = 2 * i - j + 1;
            Y[i][j] = 0;
        }
    }
    cout << "Matrix X:\n";
    outMatrix(X, N);

    cout << "\nInput number of threads: ";
    cin >> numthreads;

    tids = new pthread_t[numthreads];
    args = new thread_argument[numthreads];
    // work with threads
    pthread_attr_init(&attr);
    for (int rank = 0; rank < numthreads; rank++) {
        args[rank].start = (rank * N) / numthreads;
        args[rank].end = ((rank + 1) * N) / numthreads;
        args[rank].rank = rank;
        pthread_create(&(tids[rank]), &attr, thread_func1, &args[rank]);
        // for test:
        //cout << "\nrank: " << rank << " tid: " << tids[rank] << endl;
    }

    cout << "\n-------------------------------------------------\n";
    for (int rank = 0; rank < numthreads; rank++) {
        // for test:
        //cout << "Waiting tid: " << tids[rank] << endl;
        pthread_join(tids[rank], NULL);
    }
    delete [] tids;
    delete [] args;

    cout << "\nResult matrix Y:\n";
    outMatrix(Y, N);
    
    return 0;
}
