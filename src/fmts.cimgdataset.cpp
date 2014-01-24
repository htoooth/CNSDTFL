#include "fmts.cimgdataset.h"
#include "fmts.cimgrasterband.h"

using namespace cimg;

CPLErr cimg::CIMGDataset::RasterIO ( GDALRWFlag eRWFlag,
                                     int nXOff, int nYOff, int nXSize, int nYSize,
                                     void * pData, int nBufXSize, int nBufYSize,
                                     GDALDataType eBufType,
                                     int nBandCount, int * panBandMap,
                                     int nPixelSpace, int nLineSpace, int nBandSpace )
{

	return poGDALDataset->RasterIO ( eRWFlag,
	                                 nXOff, nYOff, nXSize, nYSize,
	                                 pData, nBufXSize, nBufYSize,
	                                 eBufType,
	                                 nBandCount, panBandMap,
	                                 nPixelSpace, nLineSpace, nBandSpace );
}

cimg::CIMGDataset * cimg::CIMGDataset::Open ( GDALOpenInfo * poOpenInfo )
{
	GDALRegister_GTiff();
	GDALRegister_BMP();
    printf("=============================");
	CIMGDataset * poCIMGDataset = new CIMGDataset();
	poCIMGDataset->poGDALDataset = ( GDALDataset * ) GDALOpen ( poOpenInfo->pszFilename, GA_ReadOnly );

	return poCIMGDataset;
}

cimg::CIMGRasterband * cimg::CIMGDataset::GetRasterBand ( int index )
{
	GDALRasterBand * poGDALRasterBand = poGDALDataset->GetRasterBand ( index );
	CIMGRasterband * poCIMGRasterBand = new CIMGRasterband ( poGDALDataset, poGDALRasterBand );
	return poCIMGRasterBand;
}


CPLErr cimg::CIMGDataset::GetGeoTransform ( double * padfTransform )
{

	padfTransform[0] = adfGeoTransform[0] ;   /* X Origin (top left corner) */
	padfTransform[1] = adfGeoTransform[1] ;   /* X Pixel size */
	padfTransform[2] = adfGeoTransform[2] ;

	padfTransform[3] = adfGeoTransform[3] ;   /* Y Origin (top left corner) */
	padfTransform[4] = adfGeoTransform[4] ;
	padfTransform[5] = adfGeoTransform[5] ;   /* Y Pixel Size */
	return CE_None;
}


cimg::CIMGDataset::CIMGDataset()
	: poGDALDataset ( NULL )
	, osPrjFilename ( NULL )
	, papszPrj ( NULL )
	, pszProjection ( NULL )
	, bNoDataSet(0)
	, dfNoDataValue(0.0)
{
	adfGeoTransform[0] = 0.0;     /* X Origin (top left corner) */
	adfGeoTransform[1] = 1.0;     /* X Pixel size */
	adfGeoTransform[2] = 0.0;

	adfGeoTransform[3] = 0.0;     /* Y Origin (top left corner) */
	adfGeoTransform[4] = 0.0;
	adfGeoTransform[5] = 1.0;     /* Y Pixel Size */
}

cimg::CIMGDataset::~CIMGDataset()
{
	GDALClose ( ( GDALDatasetH ) poGDALDataset );
}

char * cimg::CIMGDataset::GetProjectionRef()
{
	return pszProjection;
}

int cimg::CIMGDataset::RasterXSize()
{
	return poGDALDataset->GetRasterXSize();
}

int cimg::CIMGDataset::RasterYSize()
{
	return poGDALDataset->GetRasterYSize();
}

GDALDataType cimg::CIMGDataset::GetDataType()
{
	return poGDALDataset->GetRasterBand ( 1 )->GetRasterDataType();
}

int cimg::CIMGDataset::GetBandCount()
{
	return poGDALDataset->GetRasterCount();
}

double cimg::CIMGDataset::GetNoDataValue()
{
	return dfNoDataValue;
}

int cimg::CIMGDataset::GetNoDataSet()
{
	return bNoDataSet;
}

