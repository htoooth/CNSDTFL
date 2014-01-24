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
	///传递消息时的tag
	enum
	{
	    TAG_GET_DATA = 1,
	    TAG_NO_DATA = 2,
	    TAG_HAVE_DATA = 3
	};

	///划分数据
	///第一个参数为整个数据的大小
	///第二个参数为分块中x的大小
	///第三个参数为分块中y的大小
	void Partition(DatBlock oDatSize, long nXBlockSize, long nYBlockSize);

	///数据调度
	bool IsDataEmpty();
	DatBlock DataPop();
	DatBlock GetData(int did);
	void DataAdd(DatBlock dat);

	///任务调度
	bool IsNodeFree();
	int NodePop();
	void NodeAdd(int pid);
	int GetNodeSize();

	/************************************************************************/
	/* 主从式                                                                */
	/************************************************************************/

	int RunMaster(DatBlock & dat);
	int RunSlave(DatBlock & dat);

	int RunServer(DatBlock & dat);
	int StartServer(DatBlock & dat);
	int StopServer(DatBlock & dat);

	int ServerRight();
	int ServerWrong();

	/************************************************************************/
	/* 对等式                                                                */
	/************************************************************************/

	int RunPeerToPeer(DatBlock & dat);

private:
	///可工作的进程队列
	std::queue<int> m_CmpNode;

	///可计算数据队列
	std::vector<DatBlock> m_DatNode;
};

#endif