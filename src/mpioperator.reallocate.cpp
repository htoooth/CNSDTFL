#include "mpioperator.reallocate.h"

int ReAllocateRasterBlock( void * SendBuf, int SendCount, MPI_Datatype SendType,
                           void * RecvBuf, int RecvCount, MPI_Datatype RecvType,
                           MPI_Comm Comm )
{
	return MPI_Alltoall(SendBuf, SendCount, SendType,
	                    RecvBuf, RecvCount, RecvType,
	                    Comm);
}
