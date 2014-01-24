#ifndef MPI_OPERATOR_H_INCLUDE

#define MPI_OPERATOR_H_INCLUDE

#include "mpi.h"

int SendArray( MPI_Comm comm, int dest, int tag,
               void * buf, int count, MPI_Datatype datatype );

int RecvArray( MPI_Comm comm, int source, int tag,
               void * buf, int count, MPI_Datatype datatype );

MPI_Status & GetStatus();

int GetNewMsgTag();

int GetNowMsgTag();

#endif