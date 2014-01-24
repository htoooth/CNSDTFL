#include "geoalgorithm.cgrd2gdal.h"

CGRDAlgorihtm::CGRDAlgorihtm()
	: GEOAlgorithm("cgrd2gdal")
	, poSrcDS(NULL)
	, poDstDS(NULL)
	, poDstDriver(NULL)
	, pszSrcFile(NULL)
	, pszDstFile(NULL)
	, pszFormat("GTiff")
	, nBlockXSize(1)
	, nBlockYSize(1)
{

}

CGRDAlgorihtm::~CGRDAlgorihtm()
{

}

int CGRDAlgorihtm::Init( int argc, char ** argv )
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

int CGRDAlgorihtm::GetDstDriver()
{
	poDstDriver = GetRasterDriver(pszFormat);
	REQUIRE(poDstDriver != NULL);
	return GEO_ALGORITHM_RIGHT;
}

int CGRDAlgorihtm::OpenSrcDS()
{
	poSrcDS = cgrd::CGRDOpen(pszSrcFile);
	REQUIRE(poSrcDS != NULL);

	nBlockXSize = poSrcDS->GetRasterXSize();
	nBlockYSize = 1;

	return GEO_ALGORITHM_RIGHT;
}

int CGRDAlgorihtm::CloseSrcDS()
{
	cgrd::CGRDClose(poSrcDS);
	return GEO_ALGORITHM_RIGHT;
}

int CGRDAlgorihtm::CreateDstDS()
{
	int nRasterXSize = poSrcDS->GetRasterXSize();
	int nRasterYSize = poSrcDS->GetRasterYSize();
	int nBandCount = poSrcDS->GetRasterCount();

	cgrd::CGRDRasterBand * poSrcBand = poSrcDS->GetRasterBand ();
	int bSetNoData = FALSE;
	double dfNoDataReal = poSrcBand->GetNoDataValue ( &bSetNoData );
	GDALDataType eOutputType = poSrcBand->GetRasterDataType();

	char ** papszOption =
	    SetCreateMetaData(nRasterXSize, nRasterYSize, nBandCount, eOutputType);
	poDstDS = RasterCreate(pszFormat, pszDstFile, papszOption);

	CSLDestroy(papszOption);

	REQUIRE(poDstDS != NULL);
	return GEO_ALGORITHM_RIGHT;
}

int CGRDAlgorihtm::OpenDstDS()
{
	int err = GEO_ALGORITHM_WRONG;

	if (poDstDS = RasterOpen(pszDstFile, GA_Update))
	{
		err = GEO_ALGORITHM_RIGHT;
	}

	return err;
}

int CGRDAlgorihtm::CloseDstDS()
{
	RasterClose(poDstDS);
	poDstDS = NULL;
	return GEO_ALGORITHM_RIGHT;
}

int CGRDAlgorihtm::GetComputeDataSize( DatBlock & data )
{
	data.nOffX = 0;
	data.nOffY = 0;
	data.nXSize = poSrcDS->GetRasterXSize();
	data.nYSize = poDstDS->GetRasterYSize();
	return GEO_ALGORITHM_RIGHT;
}

int CGRDAlgorihtm::Compute( const DatBlock & data )
{
	int nOffX = data.nOffX;
	int nOffY = data.nOffY;
	int nXSize = data.nXSize;
	int nYSize = data.nYSize;

	cgrd::CGRDRasterBand * poSrcBand = poSrcDS->GetRasterBand();
	GDALRasterBand * poDstBand = poDstDS->GetRasterBand(1);

	GDALDataType eDatatype = poSrcBand->GetRasterDataType();
	int nDTSize = GDALGetDataTypeSize ( eDatatype ) / 8;

	int nXBlockSize = poSrcDS->BlockXSize();
	int nYBlockSize = poSrcDS->BlockYSize();

	GByte * pszData = ( GByte * ) CPLMalloc ( nXBlockSize * nYBlockSize * nDTSize );
	CPLErr err;

	for (int iY = nOffY; iY < nOffY + nYSize; ++iY)
	{
		poSrcBand->ReadBlock(nOffX, iY, pszData);
		err = poDstBand->RasterIO(GF_Write,
		                          0, iY, nXBlockSize, nYBlockSize,
		                          pszData, nXBlockSize, nYBlockSize,
		                          eDatatype, 0, 0);
	}

	CPLFree ( pszData );

	return GEO_ALGORITHM_RIGHT;
}

long CGRDAlgorihtm::GetBlockYSize()
{
	return nBlockXSize;
}

long CGRDAlgorihtm::GetBlockXSize()
{
	return nBlockYSize;
}

int CGRDAlgorihtm::Clean()
{
	RasterClean();
	return GEO_ALGORITHM_RIGHT;
}

void CGRDAlgorihtm::Help()
{
	printf(
	    "Usage: cgrd2gdal [-format <format_name>]\n"
	    "                 -src <src_filename>\n"
	    "                 -dst <dst_filename>\n" );
	exit(1);
}

