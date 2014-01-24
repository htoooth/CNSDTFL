#include "geoalgorithm.cimg2gdal.h"

using namespace cimg;

CIMGAlgorihtm::CIMGAlgorihtm()
	: GEOAlgorithm("cimg2gdal")
	, poSrcDS(NULL)
	, poDstDS(NULL)
	, poDstDriver(NULL)
	, pszSrcFile(NULL)
	, pszDstFile(NULL)
	, pszFormat("GTiff")
	, nBlockXSize(0)
	, nBlockYSize(0)
{

}

CIMGAlgorihtm::~CIMGAlgorihtm()
{

}

int CIMGAlgorihtm::Init( int argc, char ** argv )
{
	for ( int i = 1; i < argc; i++ )
	{
		if ( EQUAL(argv[i], "-format") && i < argc - 1 )
		{
			pszFormat = argv[++i];
		}

		else if ( EQUAL(argv[i], "-src")  && i < argc - 1 )
		{
			pszSrcFile = argv[++i];
		}

		else if ( EQUAL(argv[i], "-dst")  && i < argc - 1 )
		{
			pszDstFile = argv[++i];
		}

		else
			Help();
	}

	if ( pszDstFile == NULL
	        || strcmp(pszSrcFile, pszDstFile) == 0)
	{
		Help();
	}

	return GEO_ALGORITHM_RIGHT;
}

int CIMGAlgorihtm::GetDstDriver()
{
	poDstDriver = GetRasterDriver(pszFormat);

	REQUIRE(poDstDriver != NULL);

	return GEO_ALGORITHM_RIGHT;
}

int CIMGAlgorihtm::OpenSrcDS()
{
	poSrcDS = cimg::CIMGOpen(pszSrcFile);

	REQUIRE(poSrcDS != NULL);

	/************************************************************************/
	/* 划分块的大小                                                           */
	/************************************************************************/
	nBlockXSize = poSrcDS->RasterXSize();
	nBlockYSize = 1;

	return GEO_ALGORITHM_RIGHT;
}

int CIMGAlgorihtm::CloseSrcDS()
{
	ASSURE(poSrcDS != NULL);

	cimg::CIMGClose(poSrcDS);
	poSrcDS = NULL;

	return GEO_ALGORITHM_RIGHT;
}

int CIMGAlgorihtm::CreateDstDS()
{
	int nXSize = poSrcDS->RasterXSize();
	int nYSize = poSrcDS->RasterYSize();
	int nBand = poSrcDS->GetBandCount();
	GDALDataType eDataType = poSrcDS->GetDataType();

	char ** papszOption = SetCreateMetaData(nXSize, nYSize, nBand, eDataType);
	poDstDS = RasterCreate(pszFormat, pszDstFile, papszOption);

	REQUIRE(poDstDS != NULL);

	return GEO_ALGORITHM_RIGHT;
}

int CIMGAlgorihtm::OpenDstDS()
{
	poDstDS = RasterOpen(pszDstFile, GA_Update);

	if (poDstDS == NULL)
	{
		return GEO_ALGORITHM_WRONG;
	}

	return GEO_ALGORITHM_RIGHT;
}

int CIMGAlgorihtm::CloseDstDS()
{
	ASSURE(poDstDS != NULL);

	RasterClose(poDstDS);

	poDstDS = NULL;

	return GEO_ALGORITHM_RIGHT;
}

int CIMGAlgorihtm::GetComputeDataSize( DatBlock & data )
{
	data.nOffX = 0;
	data.nOffY = 0;
	data.nXSize = poSrcDS->RasterXSize();
	data.nYSize = poSrcDS->RasterYSize();

	return GEO_ALGORITHM_RIGHT;
}

int CIMGAlgorihtm::Compute( const DatBlock & data )
{
	long offx = data.nOffX;
	long offy = data.nOffY;
	long xsize = data.nXSize;
	long ysize = data.nYSize;

	GDALDataType eDataType = poSrcDS->GetDataType();
	int DataSize = GDALGetDataTypeSize(eDataType) / 8;
	int nBand = poSrcDS->GetBandCount();

	int * band = (int * )CPLMalloc(sizeof(int) * nBand);

	for (int i = 0; i < nBand; ++i)
	{
		band[i] = i + 1;
	}

	GByte * pszData = ( GByte * ) CPLMalloc ( xsize * ysize * DataSize * nBand);

	ASSURE(pszData != NULL);

	CPLErr err = poSrcDS->RasterIO(GF_Read, offx, offy, xsize, ysize,
	                               pszData, xsize, ysize,
	                               eDataType, nBand, band, 0, 0, 0);

	if (err == CE_None)
	{
		poDstDS->RasterIO(GF_Write, offx, offy, xsize, ysize,
		                  pszData, xsize, ysize,
		                  eDataType, nBand, band, 0, 0, 0);
	}

	CPLFree(pszData);

	return GEO_ALGORITHM_RIGHT;
}

long CIMGAlgorihtm::GetBlockYSize()
{
	return nBlockXSize;
}

long CIMGAlgorihtm::GetBlockXSize()
{
	return nBlockYSize;
}

int CIMGAlgorihtm::Clean()
{
	RasterClean();

	return GEO_ALGORITHM_RIGHT;
}

void CIMGAlgorihtm::Help()
{
	printf(
	    "Usage: cimg2gdal [-format <format_name>]\n"
	    "                 -src <src_filename>\n"
	    "                 -dst <dst_filename>\n" );
	exit( 1 );
}
