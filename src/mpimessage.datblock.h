#ifndef MPI_MESSAGE_DATBLOCK_H_INCLUDE

#define MPI_MESSAGE_DATBLOCK_H_INCLUDE

#include "mpi.h"
#include "port.datblock.h"

using port::DatBlock;

MPI_Datatype Commit_MPI_Type_DatBlock();
void Free_MPI_Type_DatBlock();

#endif