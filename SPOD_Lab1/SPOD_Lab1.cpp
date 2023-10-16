#include <iostream>
#include "mpi.h"

#define TEXT_SIZE 50
#define SEC_TO_MICROSEC 1'000'000
#define CHAR_VALUES_COUNT 256

int main(int argc, char* argv[])
{
    int processesCount, processRank, receivedRank;
    double start, end;
    MPI_Status status;
    char text[50];

    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &processesCount);
    MPI_Comm_rank(MPI_COMM_WORLD, &processRank);

    if (processRank == 0) {
        strcpy_s(text, "This text does not have errors!");
        printf("Before: %s\n", text);
        start = MPI_Wtime();
        if (processesCount > 1) {
            MPI_Send(text, TEXT_SIZE, MPI_CHAR, 1, 0, MPI_COMM_WORLD);
            MPI_Recv(text, TEXT_SIZE, MPI_CHAR, processesCount - 1, 0, MPI_COMM_WORLD, &status);
        }
        end = MPI_Wtime();
        printf("After: %s\n Time: %d mcs", text, (int)((end - start) * SEC_TO_MICROSEC));
    }
    else {
        MPI_Recv(text, TEXT_SIZE, MPI_CHAR, processRank - 1, 0, MPI_COMM_WORLD, &status);
        srand(time(NULL) + processRank * processesCount);
        // rand() % CHAR_VALUES_COUNT + 1 используется для того, чтобы не получить нуль-терминатор
        text[rand() % (strlen(text))] += rand() % CHAR_VALUES_COUNT + 1;
        MPI_Send(text, TEXT_SIZE, MPI_CHAR, (processRank + 1) % processesCount, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();

    return 0;
}

