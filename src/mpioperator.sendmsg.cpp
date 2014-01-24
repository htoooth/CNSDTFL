#include "mpioperator.sendmsg.h"

void SendMsg( const MPIMessage & sendmsg )
{
	SendArray(sendmsg.GetComm(),
	          sendmsg.GetProcessId(),
	          sendmsg.GetTag(),
	          sendmsg.GetBuffer(),
	          sendmsg.GetCount(),
	          sendmsg.GetDataType());
}