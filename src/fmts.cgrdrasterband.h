#ifndef CGRD_RASTE_RBAND_H_INCLUDE
#define CGRD_RASTE_RBAND_H_INCLUDE

#include "geoalgorithm.format.h"

namespace cgrd
{

class CGRDDataset;

class CGRDRasterBand
{
	friend class CGRDDataset;

public:

	CGRDRasterBand ( CGRDDataset *, int, GDALDataType );

	~CGRDRasterBand();

	double GetNoDataValue ( int * );

	CPLErr SetNoDataValue ( double );

	CPLErr ReadBlock ( int, int, void * );

	int GetnBlockXSize();

	int GetnBlockYsize();

	GDALDataType GetRasterDataType();

	int RasterXSize() const;;
	void RasterXSize(int val);;
	int RasterYSize() const;;
	void RasterYSize(int val);;

private:

	CGRDDataset  * poDS;

	int nBand;
	int nRasterXSize;
	int nRasterYSize;
	GDALDataType eDataType;

	int nBlockXSize;
	int nBlockYSize;

	int         bNoDataSet;
	double      dfNoDataValue;

	GUIntBig   *   panLineOffset;

};

}

#endif