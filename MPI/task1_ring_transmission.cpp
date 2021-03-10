#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "mpi.h"

#define BUFFSIZE 32
int main(int argc, char** argv)
{
	int numtasks, rank;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
	MPI_Status status;

	// процесс получатель сообщения
	int dst = (rank + 1) % numtasks;

	// процесс от которого ожидается сообщение
	int src = rank == 0 ? numtasks - 1 : rank - 1;

	char send_buff[BUFFSIZE] = "Hello from process ";
	_itoa(rank, send_buff + strlen("Hello from process "), 10);
	char recv_buff[BUFFSIZE];

	/* deadlock
	MPI_Recv(recv_buff, BUFFSIZE, MPI_CHAR, src, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
	MPI_Send(send_buff, strlen(send_buff) + 1, MPI_CHAR, dst, 0, MPI_COMM_WORLD); */

	// simultaneous send-recv
	MPI_Send(send_buff, strlen(send_buff) + 1, MPI_CHAR, dst, 0, MPI_COMM_WORLD);
	MPI_Recv(recv_buff, BUFFSIZE, MPI_CHAR, src, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
	
	printf("process %d received: %s\n", rank, recv_buff);
	MPI_Finalize();
}