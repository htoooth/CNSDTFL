/**
\file hpgcframework.hpgalgorithm.h
\brief �����㷨�Ĳ��з�װ��

���ļ�����Ҫ���ڰ����ڵĵ����㷨��GEOAlgorithm��װHPGAlgorithm�С�
\author htoo
\version 0.0.0.1
\date 2013-02-18
*/

#ifndef HPGC_FRAMEWORK_HPG_ALGORITHM_H_INCLUDE

#define HPGC_FRAMEWORK_HPG_ALGORITHM_H_INCLUDE

#include "mpimessage.datblock.h"
#include "mpiobject.h"
#include "port.debug.h"

template <typename GEOAlg>
class HPGAlgorithm
{
	//HPGAlgorithm�࣬˽�еĿ����Զ��庯�����ڹ����ӿ��е���
	// int Partition();//������������ݽ��л���
	// int Register();//ע���������͸�ϵͳʹ�ã����ǵ÷���һ���������͵�ֵ
	// int Read();
	// int Write();
public:
	enum
	{
	    HPG_ALG_RIGHT = 0,
	    HPG_ALG_WRONG = 1
	};

	HPGAlgorithm(): m_GeoAlgorithm() {};

	~ HPGAlgorithm() {};

	///�Բ����ĳ�ʼ�������������ݼ�
	int All_Init(int argc, char * argv[]);

	///���Դ���ݼ���������ѡ
	int All_GetSrcDriver();

	///���Ŀ�����ݼ�����
	int All_GetDstDriver();

	///��Դ���ݼ�
	int All_OpenSrc();

	///�ر�Դ���ݼ�
	int All_CloseSrc();

	///����Ŀ�����ݼ�
	int Master_CreatDst();

	///��Ŀ�����ݼ�
	int Master_OpenDst();

	///�ر�Ŀ�����ݼ�
	int Master_CloseDst();

	///��Ŀ������Դ
	int All_OpenDst();

	///�ر�Ŀ������Դ
	int All_CloseDst();

	///�������
	int All_Clean();

	///����û��Զ������ݷֿ��С
	long GetBlockXSize();

	///����û��Զ������ݷֿ��С
	long GetBlockYSize();

	/************************************************************************/
	/* ����ʽ                                                                */
	/************************************************************************/

	///���Ŀ�����ݴ�С
	int Master_GetWholeDataSize(DatBlock & data);

	///�������ݣ����м��㣬д������
	int Slave_Compute(const DatBlock & data);

	/************************************************************************/
	/* �Ե�ʽ                                                                */
	/************************************************************************/

	///���Ŀ�����ݴ�С
	int All_GetWholeDataSize(DatBlock & data);

	///�������ݣ����м��㣬д������
	int All_Compute(const DatBlock & data);

private:

	GEOAlg m_GeoAlgorithm;
};

template <typename GEOAlg>
long HPGAlgorithm<GEOAlg>::GetBlockYSize()
{
	return m_GeoAlgorithm.GetBlockXSize();
}

template <typename GEOAlg>
long HPGAlgorithm<GEOAlg>::GetBlockXSize()
{
	return m_GeoAlgorithm.GetBlockYSize();
}

template <typename GEOAlg>
int HPGAlgorithm<GEOAlg>::Master_OpenDst()
{
	return m_GeoAlgorithm.OpenDstDS();
}

template <typename GEOAlg>
int HPGAlgorithm<GEOAlg>::All_Compute( const DatBlock & data )
{
	return m_GeoAlgorithm.Compute(data);
}

template <typename GEOAlg>
int HPGAlgorithm<GEOAlg>::All_GetWholeDataSize( DatBlock & data )
{
	return m_GeoAlgorithm.GetComputeDataSize(data);
}

template <typename GEOAlg>
int HPGAlgorithm<GEOAlg>::Slave_Compute( const DatBlock & data )
{
	return m_GeoAlgorithm.Compute(data);
}

template <typename GEOAlg>
int HPGAlgorithm<GEOAlg>::Master_GetWholeDataSize( DatBlock & data )
{
	return m_GeoAlgorithm.GetComputeDataSize(data);
}

template <typename GEOAlg>
int HPGAlgorithm<GEOAlg>::All_Clean()
{
	return m_GeoAlgorithm.Clean();
}

template <typename GEOAlg>
int HPGAlgorithm<GEOAlg>::All_CloseDst()
{
	return m_GeoAlgorithm.CloseDstDS();
}

template <typename GEOAlg>
int HPGAlgorithm<GEOAlg>::All_OpenDst()
{
	return m_GeoAlgorithm.OpenDstDS();
}

template <typename GEOAlg>
int HPGAlgorithm<GEOAlg>::Master_CloseDst()
{
	return m_GeoAlgorithm.CloseDstDS();
}

template <typename GEOAlg>
int HPGAlgorithm<GEOAlg>::Master_CreatDst()
{
	return m_GeoAlgorithm.CreateDstDS();
}

template <typename GEOAlg>
int HPGAlgorithm<GEOAlg>::All_CloseSrc()
{
	return m_GeoAlgorithm.CloseSrcDS();
}

template <typename GEOAlg>
int HPGAlgorithm<GEOAlg>::All_OpenSrc()
{
	return m_GeoAlgorithm.OpenSrcDS();
}

template <typename GEOAlg>
int HPGAlgorithm<GEOAlg>::All_GetDstDriver()
{
	return m_GeoAlgorithm.GetDstDriver();
}

template <typename GEOAlg>
int HPGAlgorithm<GEOAlg>::All_GetSrcDriver()
{
	return m_GeoAlgorithm.GetSrcDriver();
}

template <typename GEOAlg>
int HPGAlgorithm<GEOAlg>::All_Init( int argc, char * argv[] )
{
	return m_GeoAlgorithm.Init(argc, argv);
}

#endif