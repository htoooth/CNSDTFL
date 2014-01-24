#include "mpioperator.recvmsg.h"

void RecvMsg( MPIMessage & recvmsg )
{
	RecvArray(recvmsg.GetComm(),
	          recvmsg.GetProcessId(),
	          recvmsg.GetTag(),
	          recvmsg.GetBuffer(),
	          recvmsg.GetCount(),
	          recvmsg.GetDataType());
}
