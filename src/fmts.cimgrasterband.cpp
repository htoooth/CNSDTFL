#include "fmts.cimgrasterband.h"
#include "fmts.cimgdataset.h"

CPLErr cimg::CIMGRasterband::RasterIO ( GDALRWFlag eRWFlag,
                                        int nXOff, int nYOff, int nXSize, int nYSize,
                                        void * pData, int nBufXSize, int nBufYSize,
                                        GDALDataType eBufType,
                                        int nPixelSpace,
                                        int nLineSpace )

{
	return poGDALRasterBand->RasterIO ( eRWFlag,
	                                    nXOff, nYOff, nXSize, nYSize,
	                                    pData, nBufXSize, nBufYSize,
	                                    eBufType,
	                                    nPixelSpace,
	                                    nLineSpace );
}

cimg::CIMGRasterband::CIMGRasterband ( GDALDataset * poDS, GDALRasterBand * poRB ) :
	poGDALRasterBand ( poRB ),
	poGDALDateset ( poDS )

{

}

cimg::CIMGRasterband::~CIMGRasterband()
{

}

GDALRasterBand * cimg::CIMGRasterband::GetRasterBand()
{
	return poGDALRasterBand;
}



