#include "framework.partition.h"
#include "mpimessage.datblock.h"

#include "port.debug.h"

void HPGPartition::Partition( DatBlock oDatSize, long nXBlockSize, long nYBlockSize )
{
	long nOffX = oDatSize.nOffX;
	long nXSize = oDatSize.nXSize;

	long nOffY = oDatSize.nOffY;
	long nYSize = oDatSize.nYSize;

	long nXBlocks = nXSize / nXBlockSize;
	long nYBlocks = nYSize / nYBlockSize;

	if (nXBlocks == 0)
	{
		nXBlocks = 1;
	}

	if (nYBlocks == 0)
	{
		nYBlocks = 1;
	}

	long nRestX = 0;
	long nRestY = 0;

	if (nXBlocks * nXBlockSize < nXSize)
	{
		nRestX = nXSize - nXBlocks * nXBlockSize;
	}

	if (nYBlocks * nYBlockSize < nYSize)
	{
		nRestY = nYSize - nYBlocks * nYBlockSize;
	}

	BUG("=========");
	BUG(oDatSize.nOffX);
	BUG(oDatSize.nOffY);
	BUG(oDatSize.nXSize);
	BUG(oDatSize.nYSize);
	BUG(nXBlockSize);
	BUG(nYBlockSize);
	BUG(nXBlocks);
	BUG(nYBlocks);
	BUG(nRestX);
	BUG(nRestY);
	BUG("=========");


	long did = 0;

	for (int iX = 0; iX < nXBlocks; ++iX )
	{
		long iXOff = nOffX + iX * nXBlockSize;

		if (iX == nXBlocks - 1)
		{
			nXBlockSize += nRestX;
		}

		for (int iY = 0; iY < nYBlocks; ++iY)
		{
			long iYOff = nOffY + iY * nYBlockSize;

			if (iY == nYBlocks - 1)
			{
				nYBlockSize += nRestY;
			}

			DatBlock dat;
			dat.DID = did++;
			dat.nOffX = iXOff;
			dat.nOffY = iYOff;
			dat.nXSize = nXBlockSize;
			dat.nYSize = nYBlockSize;

			DataAdd(dat);


			BUG("=========");
			BUG(dat.DID);
			BUG(dat.nOffX);
			BUG(dat.nOffY);
			BUG(dat.nXSize);
			BUG(dat.nYSize);
			BUG("=========");
		}
	}
}

bool HPGPartition::IsDataEmpty()
{
	return m_DatNode.empty();
}

DatBlock HPGPartition::DataPop()
{
	DatBlock dat = m_DatNode.front();
	std::vector<DatBlock>::iterator iter = m_DatNode.begin();
	m_DatNode.erase(iter);
	return dat;
}

DatBlock HPGPartition::GetData( int did )
{
	return m_DatNode.at(did);
}

void HPGPartition::DataAdd( DatBlock dat )
{
	m_DatNode.push_back(dat);
}

bool HPGPartition::IsNodeFree()
{
	return m_CmpNode.empty();
}

int HPGPartition::NodePop()
{
	int pid = m_CmpNode.front();
	m_CmpNode.pop();
	return pid;
}

void HPGPartition::NodeAdd( int pid )
{
	m_CmpNode.push(pid);
}

int HPGPartition::RunMaster( DatBlock & dat )
{
	MPIObject mo;
	MPI_Datatype datinfo = Commit_MPI_Type_DatBlock();

	MPIMessage rMsg(&dat, 1, datinfo, MPI_ANY_SOURCE, TAG_GET_DATA, mo.GetComm());
	MPIMessage sMsg(&dat, 1, datinfo, MPI_ANY_SOURCE, MPI_ANY_TAG, mo.GetComm());

	while (!IsDataEmpty() || GetNodeSize() != mo.GetOurSize() - 1)
	{
		RecvMsg(rMsg);
		NodeAdd(GetStatus().MPI_SOURCE);

		if (IsDataEmpty())
		{
			sMsg.SetProcessId(GetStatus().MPI_SOURCE);
			sMsg.SetTag(TAG_NO_DATA);
		}

		else
		{
			dat = DataPop();
			sMsg.SetProcessId(NodePop());
			sMsg.SetTag(TAG_HAVE_DATA);
		}

		SendMsg(sMsg);
	}

	return ServerWrong();
}

int HPGPartition::RunSlave( DatBlock & dat )
{
	MPIObject mo;
	MPI_Datatype datinfo = Commit_MPI_Type_DatBlock();

	MPIMessage rMsg(&dat, 1, datinfo, mo.GetMaster(), MPI_ANY_TAG, mo.GetComm());
	MPIMessage sMsg(&dat, 1, datinfo, mo.GetMaster(), TAG_GET_DATA, mo.GetComm());

	SendMsg(sMsg);
	RecvMsg(rMsg);

	MPI_Status sta = GetStatus();

	if (sta.MPI_TAG == TAG_HAVE_DATA)
	{
		return ServerRight();
	}

	else if (sta.MPI_TAG == TAG_NO_DATA)
	{
		return ServerWrong();
	}

	else
	{
		return ServerWrong();
	}
}

int HPGPartition::RunServer( DatBlock & dat )
{
	MPIObject mo;

	if (mo.IsMaster())
	{
		return RunMaster(dat);
	}

	else
	{
		return RunSlave(dat);
	}
}

int HPGPartition::StartServer( DatBlock & dat )
{
	MPIObject mo;

	if (mo.GetOurSize() == 1)
	{
		return ServerWrong();
	}

	return RunServer(dat);
}

int HPGPartition::StopServer( DatBlock & dat )
{
	return ServerWrong();
}

int HPGPartition::RunPeerToPeer( DatBlock & dat )
{
	MPIObject mo;
	dat = GetData(mo.GetMe());
	return ServerRight();
}

int HPGPartition::GetNodeSize()
{
	return m_CmpNode.size();
}

int HPGPartition::ServerRight()
{
	return MSG_HPGC_FRAMEWORK_RIGHT;
}

int HPGPartition::ServerWrong()
{
	return MSG_HPGC_FRAMEWORK_EXIT;
}
