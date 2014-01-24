#include "mpioperator.h"

static MPI_Status Status;

static int tag = 0;

int SendArray( MPI_Comm comm, int dest, int tag, void * buf, int count, MPI_Datatype datatype )
{
	return MPI_Send(buf, count, datatype, dest, tag, comm);
}

int RecvArray( MPI_Comm comm, int source, int tag, void * buf, int count, MPI_Datatype datatype )
{
	return MPI_Recv(buf, count, datatype, source, tag, comm, &Status);
}

MPI_Status & GetStatus()
{
	return Status;
}

int GetNewMsgTag()
{
	return ++tag;
}

int GetNowMsgTag()
{
	return tag;
}

