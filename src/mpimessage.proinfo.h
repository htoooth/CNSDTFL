#ifndef MPI_MESSAGE_PROINFO_H_INCLUDE

#define MPI_MESSAGE_PROINFO_H_INCLUDE

#include "mpimessage.h"

typedef struct
{
	int PID;
	int DID;
	int bSuccess;

	double dfStart;
	double dfEnd;

} ProcInfo;

MPI_Datatype Commit_MPI_Type_ProInfo();
void Free_MPI_Type_ProInfo();

#endif


