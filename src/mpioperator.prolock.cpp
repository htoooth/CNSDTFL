#include "mpioperator.prolock.h"
#include "mpiobject.h"
#include "mpiport.constant.h"
#include "mpioperator.recvmsg.h"
#include "mpioperator.sendmsg.h"
#include "mpimessage.h"

//数据信息
#define MSG_TAG_LOCK 1
#define MSG_TAG_DATA 2
#define MSG_TAG_FREE 3

#define MASTER_RANK 0
#define SLAVE_RANK 1


int ProAcquireLock( int * pMutex )
{
	MPIObject oMPI;

	if (oMPI.IsMaster())
	{
		int sig1 = 0;
		MPIMessage rMsg1(&sig1, 1, MPI_INT, MPI_ANY_SOURCE, MSG_TAG_LOCK, MPI_COMM_WORLD);
		MPIMessage rMsg2(&sig1, 1, MPI_INT, MPI_ANY_SOURCE, MSG_TAG_FREE, MPI_COMM_WORLD);

		MPIMessage sMsg(&sig1, 1, MPI_INT, MPI_ANY_SOURCE, MSG_TAG_DATA, MPI_COMM_WORLD);

		for (int ipro = 1; ipro < oMPI.GetOurSize(); ++ipro)
		{
			RecvMsg(rMsg1);

			MPI_Status status = GetStatus();
			sMsg.SetProcessId(status.MPI_SOURCE);
			SendMsg(sMsg);

			rMsg2.SetProcessId(status.MPI_SOURCE);
			RecvMsg(rMsg2);
		}

		* pMutex = MASTER_RANK;
	}

	if (oMPI.IsSlave())
	{
		int sig = 0;

		MPIMessage sMsg(&sig, 1, MPI_INT, oMPI.GetMaster(), MSG_TAG_LOCK, MPI_COMM_WORLD);
		MPIMessage rMsg(&sig, 1, MPI_INT, oMPI.GetMaster(), MSG_TAG_DATA, MPI_COMM_WORLD);

		SendMsg(sMsg);
		RecvMsg(rMsg);

		* pMutex = SLAVE_RANK;
	}

	return 1;
}

void ProReleaseLock()
{
	MPIObject oMPI;

	if (oMPI.IsSlave())
	{
		int sig = 0;
		MPIMessage sMsg(&sig, 1, MPI_INT, oMPI.GetMaster(), MSG_TAG_FREE, MPI_COMM_WORLD);
		SendMsg(sMsg);
	}
}
