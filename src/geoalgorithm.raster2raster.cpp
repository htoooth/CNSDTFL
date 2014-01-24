#include "geoalgorithm.raster2raster.h"
#include "port.debug.h"
#include "port.datblock.h"
using port::DatBlock;

static void Exit(int code)
{
	exit(code);
}

static void Usage()
{
	printf( "[ERROR]Usage:raster2raster [-of output_format_name]\n"
	        "                            src_dataset dst_dataset\n" );

	Exit(1);
}

/************************************************************************/
/*                  CreateOutputDataset()                               */
/************************************************************************/

static
GDALDatasetH CreateOutputDataset(OGRSpatialReferenceH hSRS,
                                 GDALDriverH hDriver, const char * pszDstFilename,
                                 int nXSize, int nYSize,
                                 int nBandCount, GDALDataType eOutputType,
                                 int bNoDataSet, double dfNoData)
{
	char * pszWKT = NULL;
	GDALDatasetH hDstDS = NULL;

	char ** papszCreate = NULL;
	papszCreate = CSLSetNameValue(papszCreate, "BLOCKXSIZE", "256");
	papszCreate = CSLSetNameValue( papszCreate, "BLOCKYSIZE", "1" );

	hDstDS = GDALCreate(hDriver, pszDstFilename, nXSize, nYSize,
	                    nBandCount, eOutputType, papszCreate);

	if (hDstDS == NULL)
	{
		exit(1);
	}

	if (hSRS)
	{
		OSRExportToWkt(hSRS, &pszWKT);
	}

	if (pszWKT)
	{
		GDALSetProjection(hDstDS, pszWKT);
	}

	CPLFree(pszWKT);

	if (bNoDataSet)
	{
		for (int iBand = 0; iBand < nBandCount; iBand++)
		{
			GDALRasterBandH hBand = GDALGetRasterBand(hDstDS, iBand + 1);
			GDALSetRasterNoDataValue(hBand, dfNoData);
			GDALFillRaster(hBand, 0, 0);
		}
	}

	return hDstDS;
}

/************************************************************************/
/*                           CopyBandInfo()                            */
/************************************************************************/

static
void CopyBandInfo( GDALRasterBand * poSrcBand, GDALRasterBand * poDstBand)
{
	poDstBand->SetMetadata( poSrcBand->GetMetadata() );
	poDstBand->SetColorTable( poSrcBand->GetColorTable() );
	poDstBand->SetColorInterpretation(poSrcBand->GetColorInterpretation());

	if ( strlen(poSrcBand->GetDescription()) > 0 )
	{
		poDstBand->SetDescription( poSrcBand->GetDescription() );
	}

	int bSuccess;
	double dfNoData = poSrcBand->GetNoDataValue( &bSuccess );

	if ( bSuccess )
	{
		poDstBand->SetNoDataValue( dfNoData );
	}

	poDstBand->SetOffset( poSrcBand->GetOffset() );
	poDstBand->SetScale( poSrcBand->GetScale() );
	poDstBand->SetCategoryNames( poSrcBand->GetCategoryNames() );

	if ( !EQUAL(poSrcBand->GetUnitType(), "") )
	{
		poDstBand->SetUnitType( poSrcBand->GetUnitType() );
	}
}


/************************************************************************/
/*  CopyWholeRaster()                                                   */
/************************************************************************/
CPLErr
CopyWholeRaster( GDALDatasetH hSrcDS, GDALDatasetH hDstDS, int nulx, int nuly )
{
	GDALDataset * poSrcDS = (GDALDataset *) hSrcDS;
	GDALDataset * poDstDS = (GDALDataset *) hDstDS;
	CPLErr eErr = CE_None;

	/* -------------------------------------------------------------------- */
	/*      Confirm the datasets match in size and band counts.             */
	/* -------------------------------------------------------------------- */
	int nXSize = poSrcDS->GetRasterXSize();
	int nYSize = poSrcDS->GetRasterYSize();
	int nBandCount = poSrcDS->GetRasterCount();

	if ( poDstDS->GetRasterXSize() != nXSize
	        || poDstDS->GetRasterCount() != nBandCount )
	{
		CPLError( CE_Failure, CPLE_AppDefined,
		          "Input and output dataset sizes or band counts do not match " );
		return CE_Failure;
	}

	/* -------------------------------------------------------------------- */
	/*      Get our prototype band, and assume the others are similarly     */
	/*      configured.                                                     */
	/* -------------------------------------------------------------------- */
	if ( nBandCount == 0 )
	{
		return CE_None;
	}

	GDALRasterBand * poDstPrototypeBand = poDstDS->GetRasterBand(1);
	GDALDataType eDataType = poDstPrototypeBand->GetRasterDataType();

	int nPixelSize = (GDALGetDataTypeSize(eDataType) / 8);

	void * pSwathBuf = VSIMalloc3(nXSize, nYSize, nPixelSize );

	if ( pSwathBuf == NULL )
	{
		return CE_Failure;
	}

	for ( int iBand = 0; iBand < nBandCount && eErr == CE_None; iBand++ )
	{
		int nBand = iBand + 1;

		eErr = poSrcDS->RasterIO( GF_Read,
		                          0, 0, nXSize, nYSize,
		                          pSwathBuf, nXSize, nYSize,
		                          eDataType, 1, &nBand,
		                          0, 0, 0 );

		if ( eErr == CE_None )
		{
			eErr = poDstDS->RasterIO( GF_Write,
			                          nulx, nuly, nXSize, nYSize,
			                          pSwathBuf, nXSize, nYSize,
			                          eDataType, 1, &nBand,
			                          0, 0, 0 );
		}
	}

	/* -------------------------------------------------------------------- */
	/*      Cleanup                                                         */
	/* -------------------------------------------------------------------- */
	CPLFree( pSwathBuf );

	return eErr;
}

R2RAlgorithm::R2RAlgorithm()
	: GEOAlgorithm("Raster2Raster")
	, poSrcDS(NULL), poDstDS(NULL), poDstDriver(NULL)
	, pszSrcFile(NULL), pszDstFile(NULL), pszFormat("GTiff")
	, nBandCount(0), panBandList(NULL)
{

}

int R2RAlgorithm::Init( int argc, char ** argv )
{
	if ( argc < 1 )
	{
		Usage();
	}

	for ( int i = 1; i < argc; i++ )
	{
		if ( EQUAL(argv[i], "-of") && i < argc - 1 )
		{
			pszFormat = argv[++i];
		}

		else if ( pszSrcFile == NULL )
		{
			pszSrcFile = argv[i];
		}

		else if ( pszDstFile == NULL )
		{
			pszDstFile = argv[i];
		}

		else
		{
			Usage();
		}
	}

	if ( pszDstFile == NULL
	        || strcmp(pszSrcFile, pszDstFile) == 0)
	{
		Usage();
	}

	GDALAllRegister();

	return GEO_ALGORITHM_RIGHT;
}

int R2RAlgorithm::GetDstDriver()
{
	poDstDriver = GDALGetDriverByName( pszFormat );

	if (poDstDriver == NULL)
	{
		Usage();
	}

	return GEO_ALGORITHM_RIGHT;
}

int R2RAlgorithm::OpenSrcDS()
{
	poSrcDS = GDALOpenShared( pszSrcFile, GA_ReadOnly );

	if ( poSrcDS == NULL )
	{
		Usage();
	}

	nBandCount = GDALGetRasterCount( poSrcDS );

	if ( nBandCount == 0 )
	{
		Usage();
	}

	panBandList = (int *) CPLMalloc(sizeof(int) * nBandCount);

	for ( int i = 0; i < nBandCount; i++ )
	{
		panBandList[i] = i + 1;
	}

	return GEO_ALGORITHM_RIGHT;
}

int R2RAlgorithm::CloseSrcDS()
{
	GDALClose(poSrcDS);

	return GEO_ALGORITHM_RIGHT;
}

int R2RAlgorithm::CreateDstDS()
{
	/* -------------------------------------------------------------------- */
	/*      Collect some information from the source file.                  */
	/* -------------------------------------------------------------------- */

	int nRasterXSize = GDALGetRasterXSize( poSrcDS );
	int nRasterYSize = GDALGetRasterYSize( poSrcDS );

	/************************************************************************/
	/* get raster data type                                                 */
	/************************************************************************/

	GDALRasterBand * poSrcBand = ((GDALDataset *) poSrcDS)->GetRasterBand(1);
	GDALDataType ePixelType = poSrcBand->GetRasterDataType();

	/************************************************************************/
	/* get nodata                                                           */
	/************************************************************************/

	int bSetNoData;
	double dfNoData = poSrcBand->GetNoDataValue(&bSetNoData);

	/************************************************************************/
	/* get srs                                                              */
	/************************************************************************/

	OGRSpatialReferenceH oSRS = OSRNewSpatialReference(NULL);
	const char * pszProjection = GDALGetProjectionRef( poSrcDS );

	if ( pszProjection != NULL && strlen(pszProjection) > 0 )
	{
		OSRSetFromUserInput(oSRS, pszProjection);
	}

	/************************************************************************/
	/* create dataset                                                       */
	/************************************************************************/

	poDstDS = CreateOutputDataset( oSRS, poDstDriver, pszDstFile,
	                               nRasterXSize,  nRasterYSize,
	                               nBandCount,  ePixelType,
	                               bSetNoData,  dfNoData);

	/************************************************************************/
	/* set metadata                                                         */
	/************************************************************************/

	((GDALDataset *)poDstDS)->SetMetadata(((GDALDataset *)poSrcDS)->GetMetadata());

	return GEO_ALGORITHM_RIGHT;
}

int R2RAlgorithm::OpenDstDS()
{
	poDstDS = GDALOpen( pszDstFile, GA_Update );

	if (poDstDS == NULL)
	{
		return GEO_ALGORITHM_WRONG;
	}

	else
	{
		return GEO_ALGORITHM_RIGHT;
	}
}

int R2RAlgorithm::CloseDstDS()
{
	GDALFlushCache(poDstDS);
	GDALClose(poDstDS);
	poDstDS = NULL;

	return GEO_ALGORITHM_RIGHT;
}

int R2RAlgorithm::GetComputeDataSize( DatBlock & data )
{
	int nRasterXSize = GDALGetRasterXSize( poSrcDS );
	int nRasterYSize = GDALGetRasterYSize( poSrcDS );

	data.nOffX = 0;
	data.nOffY = 0;
	data.nXSize = nRasterXSize;
	data.nYSize = nRasterYSize;

	return GEO_ALGORITHM_RIGHT;
}

int R2RAlgorithm::Compute( const DatBlock & data )
{
	long nXSize = data.nXSize;
	long nYSize = data.nYSize;
	long OffX = data.nOffX;
	long OffY = data.nOffY;

	//--------------------------------------------------------------------
	// Create a virtual dataset.
	//--------------------------------------------------------------------

	VRTDataset * poVRTDS = (VRTDataset *) VRTCreate( nXSize, nYSize );

	/************************************************************************/
	/*        Process all bands.                                            */
	/************************************************************************/
	for ( int iBand = 0; iBand < nBandCount; iBand++ )
	{
		int nSrcBand = panBandList[iBand];

		GDALRasterBand * poSrcBand = ((GDALDataset *) poSrcDS)->GetRasterBand(ABS(nSrcBand));
		GDALDataType ePixelType = poSrcBand->GetRasterDataType();
		// --------------------------------------------------------------------
		//     Create this band.
		// --------------------------------------------------------------------
		poVRTDS->AddBand( ePixelType, NULL );
		VRTSourcedRasterBand * poVRTBand = (VRTSourcedRasterBand *)
		                                   poVRTDS->GetRasterBand( iBand + 1 );

		poVRTBand->AddSimpleSource( poSrcBand, OffX, OffY, nXSize, nYSize,
		                            0, 0, nXSize, nYSize );

		CopyBandInfo(poSrcBand, poVRTBand);
	}

	CopyWholeRaster(poVRTDS, poDstDS, OffX, OffY);

	GDALFlushCache(poDstDS);

	GDALClose( (GDALDatasetH) poVRTDS );

	return GEO_ALGORITHM_RIGHT;
}

int R2RAlgorithm::Clean()
{
	GDALDestroyDriverManager();

	return GEO_ALGORITHM_RIGHT;
}

void R2RAlgorithm::Help()
{
	printf( "Usage:raster2raster [-of output_format_name]\n"
	        "                     src_dataset dst_dataset\n" );
}

R2RAlgorithm::~R2RAlgorithm()
{
	CPLFree(panBandList);
}

