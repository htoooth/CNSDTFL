/**
\file hpgcframework.hpgalgorithm.h
\brief 地理算法的并行封装类

该文件的主要用于把现在的地理算法类GEOAlgorithm封装HPGAlgorithm中。
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
	//HPGAlgorithm类，私有的可以自定义函数，在公共接口中调用
	// int Partition();//根据情况对数据进行划分
	// int Register();//注册数据类型给系统使用，可是得返回一个数据类型的值
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

	///对参数的初始化，不包括数据集
	int All_Init(int argc, char * argv[]);

	///获得源数据集驱动，可选
	int All_GetSrcDriver();

	///获得目标数据集驱动
	int All_GetDstDriver();

	///打开源数据集
	int All_OpenSrc();

	///关闭源数据集
	int All_CloseSrc();

	///创建目标数据集
	int Master_CreatDst();

	///打开目标数据集
	int Master_OpenDst();

	///关闭目标数据集
	int Master_CloseDst();

	///打开目标数据源
	int All_OpenDst();

	///关闭目标数据源
	int All_CloseDst();

	///清理操作
	int All_Clean();

	///获得用户自定义数据分块大小
	long GetBlockXSize();

	///获得用户自定义数据分块大小
	long GetBlockYSize();

	/************************************************************************/
	/* 主从式                                                                */
	/************************************************************************/

	///获得目标数据大小
	int Master_GetWholeDataSize(DatBlock & data);

	///读入数据，进行计算，写入数据
	int Slave_Compute(const DatBlock & data);

	/************************************************************************/
	/* 对等式                                                                */
	/************************************************************************/

	///获得目标数据大小
	int All_GetWholeDataSize(DatBlock & data);

	///读入数据，进行计算，写入数据
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