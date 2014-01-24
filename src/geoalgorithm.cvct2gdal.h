#ifndef GEO_ALGORITHM_CVCT2GDAL_H_INCLUDE

#define GEO_ALGORITHM_CVCT2GDAL_H_INCLUDE

#include "geoalgorithm.h"
#include "geoalgorithm.format.h"

#include "fmts.cvct.h"

class CVCTAlgorithm : public GEOAlgorithm
{
public:
	CVCTAlgorithm();
	~CVCTAlgorithm();

	///初始化参数，必选
	int Init(int argc, char ** argv);

	///获得目标数据集驱动，必选
	int GetDstDriver();

	///打开源数据集，必选
	int OpenSrcDS();

	///关闭源数据集，必选
	int CloseSrcDS();

	///创建并打开目标数据集，必选
	int CreateDstDS();

	///打开目标数据集，必选
	int OpenDstDS();

	///关闭目标数据集，必选
	int CloseDstDS();

	///获得处理数据大小，必选
	int GetComputeDataSize(DatBlock & data);

	///核心算法，必选
	int Compute(const DatBlock & data);

	///一次只处理一个图层
	long GetBlockYSize();

	///一次只处理一个图层
	long GetBlockXSize();

	///清理，必选
	int Clean();

	///打印帮助
	void Help();

private:
	cvct::VCTDataSource	*	poSrcDS ;
	OGRDataSource     *     poDstDS ;
	OGRSFDriver      *      poDstDriver;
	const char		  *  	pszSrcFile ;
	const char       *      pszDstFile ;
	const char       *      pszFormat ;
	char         **         papszLayers;

	const long              nBlockXSize;
	const long              nBlockYSize;
};

#endif
