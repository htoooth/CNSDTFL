#ifndef CIMG_DATASET_H_INCLUDE

#define CIMG_DATASET_H_INCLUDE

#include "geoalgorithm.format.h"

namespace cimg
{

class CIMGRasterband;

class CIMGDataset
{
	friend class CIMGRasterband;

public:

	CIMGDataset();

	~CIMGDataset();

	static CIMGDataset * Open ( GDALOpenInfo * poOpenInfo );

	CPLErr	RasterIO ( GDALRWFlag, int, int, int, int,
	                   void *, int, int, GDALDataType,
	                   int, int *, int, int, int );

	CIMGRasterband * GetRasterBand ( int index );

	CPLErr GetGeoTransform ( double * );

	char * GetProjectionRef();

	int RasterXSize();

	int RasterYSize();

	GDALDataType GetDataType();

	int GetBandCount();

	double GetNoDataValue();

	int GetNoDataSet();

private:
	GDALDataset * poGDALDataset;

	double adfGeoTransform[6];

	const char * osPrjFilename;

	char ** papszPrj;//坐标系字符，
	char  * pszProjection;//坐标系字符，ogr格式

	int     bNoDataSet;
	double  dfNoDataValue;
};

}

#endif