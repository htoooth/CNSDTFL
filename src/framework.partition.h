#ifndef HPGC_FRAMEWORK_PARTITION_H_INCLUDE

#define HPGC_FRAMEWORK_PARTITION_H_INCLUDE

#include "mpiobject.h"

#include "mpioperator.h"
#include "mpioperator.recvmsg.h"
#include "mpioperator.sendmsg.h"
#include "mpimessage.h"
#include "mpiport.constant.h"

#include "port.datblock.h"
#include <queue>
#include <vector>

using port::DatBlock;

class HPGPartition
{
public:
	///������Ϣʱ��tag
	enum
	{
	    TAG_GET_DATA = 1,
	    TAG_NO_DATA = 2,
	    TAG_HAVE_DATA = 3
	};

	///��������
	///��һ������Ϊ�������ݵĴ�С
	///�ڶ�������Ϊ�ֿ���x�Ĵ�С
	///����������Ϊ�ֿ���y�Ĵ�С
	void Partition(DatBlock oDatSize, long nXBlockSize, long nYBlockSize);

	///���ݵ���
	bool IsDataEmpty();
	DatBlock DataPop();
	DatBlock GetData(int did);
	void DataAdd(DatBlock dat);

	///�������
	bool IsNodeFree();
	int NodePop();
	void NodeAdd(int pid);
	int GetNodeSize();

	/************************************************************************/
	/* ����ʽ                                                                */
	/************************************************************************/

	int RunMaster(DatBlock & dat);
	int RunSlave(DatBlock & dat);

	int RunServer(DatBlock & dat);
	int StartServer(DatBlock & dat);
	int StopServer(DatBlock & dat);

	int ServerRight();
	int ServerWrong();

	/************************************************************************/
	/* �Ե�ʽ                                                                */
	/************************************************************************/

	int RunPeerToPeer(DatBlock & dat);

private:
	///�ɹ����Ľ��̶���
	std::queue<int> m_CmpNode;

	///�ɼ������ݶ���
	std::vector<DatBlock> m_DatNode;
};

#endif