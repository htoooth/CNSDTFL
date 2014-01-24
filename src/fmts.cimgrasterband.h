#ifndef CIMG_RASTER_BAND_H_INCLUDE

#define CIMG_RASTER_BAND_H_INCLUDE

#include "geoalgorithm.format.h"

namespace cimg
{

class CIMGDataset;

class CIMGRasterband
{
	friend class CIMGDataset;

public:

	CIMGRasterband ( GDALDataset * poNewDS, GDALRasterBand * poNewBand );

	~CIMGRasterband();

	CPLErr  RasterIO ( GDALRWFlag, int, int, int, int,
	                   void *, int, int, GDALDataType,
	                   int, int );

	GDALRasterBand * GetRasterBand();

private:

	CIMGRasterband();

	GDALRasterBand * poGDALRasterBand;

	GDALDataset   *  poGDALDateset;

};

}

#endif