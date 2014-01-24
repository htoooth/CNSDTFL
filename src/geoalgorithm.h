/**
\file geoalgorithm.h
\brief 地理算法的封装类

该文件的主要用于把现在的地理算法封装GEOAlgorithm中。
\author htoo
\version 0.0.0.1
\date 2013-02-18
*/

#ifndef GEO_ALGORITHM_H_INCLUDE

#define GEO_ALGORITHM_H_INCLUDE

#include "port.datblock.h"

using port::DatBlock;

class GEOAlgorithm
{
public:
	enum
	{
	    GEO_ALGORITHM_RIGHT = 0,
	    GEO_ALGORITHM_WRONG = 1
	};

	///初始化算法名
	GEOAlgorithm(const char *);

	virtual ~GEOAlgorithm();

	///初始化参数，必选
	virtual int Init(int argc, char ** argv);

	///获得源数据集驱动，可选
	virtual int GetSrcDriver();

	///获得目标数据集驱动，必选
	virtual int GetDstDriver();

	///打开源数据集，必选
	virtual int OpenSrcDS();

	///关闭源数据集，必选
	virtual int CloseSrcDS();

	///创建并打开目标数据集，必选
	virtual int CreateDstDS();

	///打开目标数据集，必选
	virtual int OpenDstDS();

	///创建或打开目标数据集，已实现
	/** \sa GEOAlgorithm::OpenDstDS GEOAlgorithm::CreateDstDS */
	int CreateOrOpenDstDS();

	///关闭目标数据集，必选
	virtual int CloseDstDS();

	///获得处理数据大小，必选
	virtual int GetComputeDataSize(DatBlock & data);

	///核心算法，必选
	virtual int Compute(const DatBlock & data);

	///清理，必选
	virtual int Clean();

	///打印帮助
	virtual void Help();

	///运行算法，测试接口，已实现
	int Test(int , char ** );

	///由用户指定数据块的大小，没有指定则返回零
	virtual long GetBlockXSize();

	///由用户指定数据块的大小，没有指定则返回零
	virtual long GetBlockYSize();

private:
	const char * m_algname;
};

#endif