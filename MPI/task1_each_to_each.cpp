#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "mpi.h"

#define BUFFSIZE 32
#define MAXTASKS 8


// предполагаем, что numtasks <= 10
void prepare_msg(char send_buff[BUFFSIZE], int rank)
{
	const char* msg = "Hello from process ";
	strcpy(send_buff, msg);
	int len = strlen(msg);
	send_buff[len] = '0' + rank;
	send_buff[len + 1] = '\0';
}



int main(int argc, char** argv)
{
	int numtasks, rank;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
	MPI_Status status;
	
	// буферы для приема и отправки от каждого процесса к каждому 
	char send_buff[BUFFSIZE];
	char recv_buff[MAXTASKS][BUFFSIZE];

	prepare_msg(send_buff, rank);

	// процесс отправляет сообщения всем остальным
	for (int i = 0; i < numtasks; ++i)
	{
		if (i != rank)
		{
			MPI_Send(send_buff, strlen(send_buff) + 1, MPI_CHAR, i, 0, MPI_COMM_WORLD);
		}	
	}
	// процесс получает сообщения от всех остальных
	for (int i = 0; i < numtasks; ++i)
	{
		if (i != rank)
		{
			MPI_Recv(recv_buff[i], BUFFSIZE, MPI_CHAR, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
			printf("process %d received: %s\n", rank, recv_buff[i]);
		}

	}
	
	MPI_Finalize();
}