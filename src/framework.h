#ifndef HPGC_FRAMEWORK_H_INCLUDE

#define	HPGC_FRAMEWORK_H_INCLUDE

#include "framework.algorithm.h"
#include "framework.partition.h"

#include "mpiobject.h"
#include "mpiport.constant.h"
#include "port.debug.h"

#include "geoalgorithm.h"

template < typename HPGAlg  = HPGAlgorithm<GEOAlgorithm>,
         typename HPGPart = HPGPartition >
class HPGCFrameWork
{
public:

	int Init(int MPI_argc, char ** MPI_argv);

	int PreAlg();

	///������ʽ�����㷨
	int RunAlgorithmInMasterAndSlave();

	///�ԶԵ�ʽ�����㷨
	int RunAlgorithmInPeerToPeer();

	///�Դ��������㷨
	int RunAlgorithm();
private:

	HPGAlg m_HPGAlgorithm;
	HPGPart m_HPGPartition;
};

template <typename HPGAlg , typename HPGPart>
int HPGCFrameWork<HPGAlg, HPGPart>::RunAlgorithm()
{
	PreAlg();
	DatBlock oDataSize ;
	m_HPGAlgorithm.All_GetWholeDataSize(oDataSize);
	m_HPGAlgorithm.All_Compute(oDataSize);
	return MSG_HPGC_FRAMEWORK_RIGHT;
}

template <typename HPGAlg , typename HPGPart>
int HPGCFrameWork<HPGAlg, HPGPart>::Init( int MPI_argc, char ** MPI_argv )
{
	m_HPGAlgorithm.All_Init(MPI_argc, MPI_argv);
	return MSG_HPGC_FRAMEWORK_RIGHT;
}

template <typename HPGAlg, typename HPGPart>
int HPGCFrameWork<HPGAlg, HPGPart>::PreAlg()
{
	m_HPGAlgorithm.All_GetSrcDriver();
	m_HPGAlgorithm.All_GetDstDriver();

	m_HPGAlgorithm.All_OpenSrc();

	MPIObject mo;

	///!!!�����ȴ����ݼ���������Ϊ���򴴽����ݼ������ݼ��Ļ�
	if (mo.IsMaster())
	{
		if (m_HPGAlgorithm.Master_OpenDst())
		{
			m_HPGAlgorithm.Master_CreatDst();
		}

		m_HPGAlgorithm.Master_CloseDst();
	}

	mo.GetBarrier();
	m_HPGAlgorithm.All_OpenDst();
	return MSG_HPGC_FRAMEWORK_RIGHT;
}

template <typename HPGAlg , typename HPGPart >
int HPGCFrameWork<HPGAlg, HPGPart>::RunAlgorithmInPeerToPeer()
{
	PreAlg();
	DatBlock oDataSize ;
	m_HPGAlgorithm.All_GetWholeDataSize(oDataSize);

	MPIObject mo;
	m_HPGPartition.Partition(oDataSize, oDataSize.nXSize, oDataSize.nYSize / mo.GetOurSize());

	DatBlock idata;
	m_HPGPartition.RunPeerToPeer(idata);

	/************************************************************************/
	/* debug                                                                */
	/************************************************************************/
	BUG("=========");
	BUG("Pass Data");
	BUG(mo.GetMe());
	BUG(idata.nOffX);
	BUG(idata.nOffY);
	BUG(idata.nXSize);
	BUG(idata.nYSize);
	BUG("=========");

	m_HPGAlgorithm.All_Compute(idata);

	mo.GetBarrier();

	m_HPGAlgorithm.All_CloseSrc();
	m_HPGAlgorithm.All_CloseDst();
	m_HPGAlgorithm.All_Clean();

	return MSG_HPGC_FRAMEWORK_RIGHT;
}

template <typename HPGAlg, typename HPGPart>
int HPGCFrameWork<HPGAlg, HPGPart>::RunAlgorithmInMasterAndSlave()
{
	PreAlg();
	DatBlock oDataSize ;
	m_HPGAlgorithm.Master_GetWholeDataSize(oDataSize);

	///����û����趨�Լ��Ŀ�Ĵ�С��ʹ��
	if (m_HPGAlgorithm.GetBlockXSize())
	{
		m_HPGPartition.Partition(oDataSize,
		                         m_HPGAlgorithm.GetBlockXSize(),
		                         m_HPGAlgorithm.GetBlockYSize());
	}

	///û�У�ʹ��ϵͳ�Դ�
	else
	{
		m_HPGPartition.Partition(oDataSize, oDataSize.nXSize, 100);
	}

	MPIObject mo;

	DatBlock idata;

	for (int algStatus = m_HPGPartition.StartServer(idata);
	        algStatus != m_HPGPartition.StopServer(idata);
	        algStatus = m_HPGPartition.RunServer(idata))
	{
		BUG("=========");
		BUG("Pass Data");
		BUG(mo.GetMe());
		BUG(idata.nOffX);
		BUG(idata.nOffY);
		BUG(idata.nXSize);
		BUG(idata.nYSize);
		BUG("=========");

		m_HPGAlgorithm.Slave_Compute(idata);
	}

	///��ȫ��������
	///����ȴ�
	mo.GetBarrier();

	m_HPGAlgorithm.All_CloseSrc();
	m_HPGAlgorithm.All_CloseDst();
	m_HPGAlgorithm.All_Clean();

	return MSG_HPGC_FRAMEWORK_RIGHT;
}

#endif
