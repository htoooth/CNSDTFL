#include "fmts.cgrddataset.h"
#include "fmts.cgrdrasterband.h"

cgrd::CGRDDataset::CGRDDataset()
{
	fp = NULL;
	poBand = NULL;

	adfGeoTransform[0] = 0.0;
	adfGeoTransform[1] = 1.0;
	adfGeoTransform[2] = 0.0;
	adfGeoTransform[3] = 0.0;
	adfGeoTransform[4] = 0.0;
	adfGeoTransform[5] = 1.0;

	papszPrj = NULL;
	pszProjection = NULL;

	bNoDataSet = TRUE;
	dfNoDataValue = -9999;

	nBufferOffset = 0;
	nOffsetInBuffer = 256;

	nRasterXSize = 0;
	nRasterYSize = 0;

	nBlockXSize = 0;
	nBlockYSize = 0;

	nHZoom = 1;

	nBandCount = 1;
}

cgrd::CGRDDataset::~CGRDDataset()
{
	if ( fp != NULL )
		VSIFCloseL ( fp );

	if ( pszProjection == NULL || papszPrj == NULL )
	{
		return;
	}

	CPLFree ( pszProjection );
	CSLDestroy ( papszPrj );
}

char cgrd::CGRDDataset::Getc()
{
	if ( nOffsetInBuffer < ( int ) sizeof ( achReadBuf ) )
	{
		return achReadBuf[nOffsetInBuffer++];

	}

	nBufferOffset = VSIFTellL ( fp );

	int nRead = VSIFReadL ( achReadBuf, 1, sizeof ( achReadBuf ), fp );

	for ( unsigned int i = nRead; i < sizeof ( achReadBuf ); i++ )
	{
		achReadBuf[i] = '\0';
	}

	nOffsetInBuffer = 0;

	return achReadBuf[nOffsetInBuffer++];
}

GUIntBig cgrd::CGRDDataset::Tell()
{
	return nBufferOffset + nOffsetInBuffer;
}

int cgrd::CGRDDataset::Seek ( GUIntBig nNewOffset )
{
	nOffsetInBuffer = sizeof ( achReadBuf );
	return VSIFSeekL ( fp, nNewOffset, SEEK_SET );
}

CPLErr cgrd::CGRDDataset::GetGeoTransform ( double * padfTransform )
{
	memcpy ( padfTransform, adfGeoTransform, sizeof ( double ) * 6 );
	return ( CE_None );
}

const char * cgrd::CGRDDataset::GetProjectionRef()
{
	return pszProjection;
}

void cgrd::CGRDDataset::SetnRasterXSize( int rasterxsize )
{
	nRasterXSize = rasterxsize;
}

void cgrd::CGRDDataset::SetnRasterYSize( int rasterysize )
{
	nRasterYSize = rasterysize;
}

void cgrd::CGRDDataset::SetnHZoom( int nhzoom )
{
	nHZoom = nhzoom;
}

int cgrd::CGRDDataset::GetnHZoom()
{
	return nHZoom;
}

cgrd::CGRDDataset * cgrd::CGRDDataset::Open ( GDALOpenInfo * poOpenInfo )
{
	char ** papszTokens = NULL;

	/************************************************************************/
	/*      Does this look like a grid file?                                */
	/************************************************************************/

	if ( poOpenInfo->nHeaderBytes < 100
	        || ! ( EQUALN ( ( const char * ) poOpenInfo->pabyHeader, "DataMark", 8 ) ||
	               EQUALN ( ( const char * ) poOpenInfo->pabyHeader, "Version", 7 ) ||
	               EQUALN ( ( const char * ) poOpenInfo->pabyHeader, "Alpha", 5 ) ||
	               EQUALN ( ( const char * ) poOpenInfo->pabyHeader, "Compress", 8 ) ||
	               EQUALN ( ( const char * ) poOpenInfo->pabyHeader, "X0", 2 ) ||
	               EQUALN ( ( const char * ) poOpenInfo->pabyHeader, "Y0", 2 ) ||
	               EQUALN ( ( const char * ) poOpenInfo->pabyHeader, "DX", 2 ) ||
	               EQUALN ( ( const char * ) poOpenInfo->pabyHeader, "DY", 2 ) ||
	               EQUALN ( ( const char * ) poOpenInfo->pabyHeader, "Row", 3 ) ||
	               EQUALN ( ( const char * ) poOpenInfo->pabyHeader, "Col", 3 ) ||
	               EQUALN ( ( const char * ) poOpenInfo->pabyHeader, "ValueType", 9 ) ||
	               EQUALN ( ( const char * ) poOpenInfo->pabyHeader, "Hzoom", 5 ) ||
	               EQUALN ( ( const char * ) poOpenInfo->pabyHeader, "XYUnit", 6 ) ||
	               EQUALN ( ( const char * ) poOpenInfo->pabyHeader, "ZUnit", 5 ) ) )
	{
		return NULL;
	}

	papszTokens = CSLTokenizeString2 ( ( const char * ) poOpenInfo->pabyHeader,
	                                   ":\n\r\t", 0 );
	int nTokens = CSLCount ( papszTokens );

	/************************************************************************/
	/*      Create a corresponding GDALDataset.                             */
	/************************************************************************/

	CGRDDataset * poDS = new CGRDDataset();

	/************************************************************************/
	/*      Parse the header.                                               */
	/************************************************************************/

	double dfCellDX = 0;
	double dfCellDY = 0;
	double dfWinULX = 0;
	double dfWinULY = 0;
	int bcompress = 0;
	int nDataType = -1; //char =0 ,integer=1
	int XYUnit; //d=0,m=1,other=2
	int ZUnit;  //d=0,m=1,other=2

	int i;

	if ( ( i = CSLFindString ( papszTokens, "DataMark" ) ) < 0 ||
	        i + 1 >= nTokens )
	{
		CSLDestroy ( papszTokens );
		delete poDS;
		return NULL;
	}

	if ( ( i = CSLFindString ( papszTokens, "Version" ) ) < 0 ||
	        i + 1 >= nTokens )
	{
		CSLDestroy ( papszTokens );
		delete poDS;
		return NULL;
	}

	if ( ( i = CSLFindString ( papszTokens, "Alpha" ) ) < 0 ||
	        i + 1 >= nTokens )
	{
		CSLDestroy ( papszTokens );
		delete poDS;
		return NULL;
	}

	if ( ( i = CSLFindString ( papszTokens, "Compress" ) ) < 0 ||
	        i + 1 >= nTokens )
	{
		CSLDestroy ( papszTokens );
		delete poDS;
		return NULL;
	}

	bcompress = atoi ( papszTokens[i + 1] );

	if ( ( i = CSLFindString ( papszTokens, "X0" ) ) < 0 ||
	        i + 1 >= nTokens )
	{
		CSLDestroy ( papszTokens );
		delete poDS;
		return NULL;
	}

	dfWinULX = CPLAtofM ( papszTokens[i + 1] );

	if ( ( i = CSLFindString ( papszTokens, "Y0" ) ) < 0 ||
	        i + 1 >= nTokens )
	{
		CSLDestroy ( papszTokens );
		delete poDS;
		return NULL;
	}

	dfWinULY = CPLAtofM ( papszTokens[i + 1] );

	if ( ( i = CSLFindString ( papszTokens, "DX" ) ) < 0 ||
	        i + 1 >= nTokens )
	{
		CSLDestroy ( papszTokens );
		delete poDS;
		return NULL;
	}

	dfCellDX = CPLAtofM ( papszTokens[i + 1] );

	if ( ( i = CSLFindString ( papszTokens, "DY" ) ) < 0 ||
	        i + 1 >= nTokens )
	{
		CSLDestroy ( papszTokens );
		delete poDS;
		return NULL;
	}

	dfCellDY = CPLAtofM ( papszTokens[i + 1] );

	if ( ( i = CSLFindString ( papszTokens, "Row" ) ) < 0 ||
	        i + 1 >= nTokens )
	{
		CSLDestroy ( papszTokens );
		delete poDS;
		return NULL;
	}

	poDS->SetnRasterYSize ( atoi ( papszTokens[i + 1] ) );

	if ( ( i = CSLFindString ( papszTokens, "Col" ) ) < 0 ||
	        i + 1 >= nTokens )
	{
		CSLDestroy ( papszTokens );
		delete poDS;
		return NULL;
	}

	poDS->SetnRasterXSize ( atoi ( papszTokens[i + 1] ) );

	poDS->BlockXSize(poDS->GetRasterXSize());
	poDS->BlockYSize(1);

	if ( ( i = CSLFindString ( papszTokens, "ValueType" ) ) < 0 ||
	        i + 1 >= nTokens )
	{
		CSLDestroy ( papszTokens );
		delete poDS;
		return NULL;
	}

	if ( EQUAL ( papszTokens[i + 1], "Char" ) )
	{
		nDataType = 0;
	}

	else if ( EQUAL ( papszTokens[i + 1], "Integer" ) )
	{
		nDataType = 1;
	}

	if ( ( i = CSLFindString ( papszTokens, "Hzoom" ) ) < 0 ||
	        i + 1 >= nTokens )
	{
		CSLDestroy ( papszTokens );
		delete poDS;
		return NULL;
	}

	poDS->SetnHZoom ( atoi ( papszTokens[i + 1] ) );

	if ( ( i = CSLFindString ( papszTokens, "XYUnit" ) ) < 0 ||
	        i + 1 >= nTokens )
	{
		CSLDestroy ( papszTokens );
		delete poDS;
		return NULL;
	}

	if ( EQUAL ( papszTokens[i + 1], "M" ) )
	{
		XYUnit = 1;
	}

	else if ( EQUAL ( papszTokens[i + 1], "D" ) )
	{
		XYUnit = 0;
	}

	else
	{
		XYUnit = 2;
	}

	if ( ( i = CSLFindString ( papszTokens, "ZUnit" ) ) < 0 ||
	        i + 1 >= nTokens )
	{
		CSLDestroy ( papszTokens );
		delete poDS;
		return NULL;
	}

	if ( EQUAL ( papszTokens[i + 1], "M" ) )
	{
		ZUnit = 1;
	}

	else if ( EQUAL ( papszTokens[i + 1], "D" ) )
	{
		ZUnit = 0;
	}

	else
	{
		ZUnit = 2;
	}

	poDS->adfGeoTransform[0] = dfWinULX;
	poDS->adfGeoTransform[1] = dfCellDX;
	poDS->adfGeoTransform[2] = 0.0;
	poDS->adfGeoTransform[3] = dfWinULY;
	poDS->adfGeoTransform[4] = 0.0;
	poDS->adfGeoTransform[5] = dfCellDY;
	poDS->bNoDataSet = TRUE;

	GDALDataType eDataType;

	if ( poDS->GetnHZoom() >= 100 )
	{
		eDataType = GDT_Float64;
	}

	else
	{
		eDataType = GDT_Int32;
	}

	/************************************************************************/
	/*    Default nodata value in grd is -9999. You can set by yourself.    */
	/************************************************************************/

	int nodata = -9999;
	poDS->dfNoDataValue = ( double ) ( nodata / poDS->GetnHZoom() );

	/************************************************************************/
	/*     Open file with large file API.                                   */
	/************************************************************************/

	poDS->fp = VSIFOpenL ( poOpenInfo->pszFilename, "r" );

	if ( poDS->fp == NULL )
	{
		CPLError ( CE_Failure, CPLE_OpenFailed,
		           "VSIFOpenL(%s) failed unexpectedly.",
		           poOpenInfo->pszFilename );
		delete poDS;
		return NULL;
	}

	/************************************************************************/
	/*      Find the start of real data.                                    */
	/************************************************************************/

	int  nStartOfData = 0;

	for ( int i = 1; TRUE ; i++ )
	{
		if ( poOpenInfo->pabyHeader[i] == '\0' )
		{
			CPLError ( CE_Failure, CPLE_AppDefined,
			           "Couldn't find data values in dem file.\n" );
			delete poDS;
			return NULL;
		}

		if ( poOpenInfo->pabyHeader[i - 1] == '\n'
		        || poOpenInfo->pabyHeader[i - 1] == '\r' )
		{
			if ( !isalpha ( poOpenInfo->pabyHeader[i] )
			        && poOpenInfo->pabyHeader[i] != '\n'
			        && poOpenInfo->pabyHeader[i] != '\r' )
			{
				nStartOfData = i;

				/* Beginning of real data found. */
				break;
			}
		}
	}

	/************************************************************************/
	/*     Open file with large file API.                                   */
	/************************************************************************/

	poDS->fp = VSIFOpenL ( poOpenInfo->pszFilename, "r" );

	if ( poDS->fp == NULL )
	{
		CPLError ( CE_Failure, CPLE_OpenFailed,
		           "VSIFOpenL(%s) failed unexpectedly.",
		           poOpenInfo->pszFilename );
		delete poDS;
		return NULL;
	}

	/************************************************************************/
	/*      Create band information objects.                                */
	/************************************************************************/

	CGRDRasterBand * poNewBand =
	    new CGRDRasterBand ( poDS, nStartOfData, eDataType );

	poDS->SetBand ( poNewBand );

	if ( poNewBand->panLineOffset == NULL )
	{
		delete poDS;
		return NULL;
	}

	CSLDestroy ( papszTokens );
	return poDS;
}

int cgrd::CGRDDataset::GetRasterXSize()
{
	return nRasterXSize;
}

int cgrd::CGRDDataset::GetRasterYSize()
{
	return nRasterYSize;
}

cgrd::CGRDRasterBand * cgrd::CGRDDataset::GetRasterBand()
{
	return poBand;
}

int cgrd::CGRDDataset::GetRasterCount()
{
	return nBandCount;
}

void cgrd::CGRDDataset::SetBand( CGRDRasterBand * poNewBand )
{
	REQUIRE(poBand == NULL);

	if (poBand == NULL)
	{
		poBand = poNewBand;
	}
}

int cgrd::CGRDDataset::BlockXSize() const
{
	return nBlockXSize;
}

void cgrd::CGRDDataset::BlockXSize( int val )
{
	nBlockXSize = val;
}

int cgrd::CGRDDataset::BlockYSize() const
{
	return nBlockYSize;
}

void cgrd::CGRDDataset::BlockYSize( int val )
{
	nBlockYSize = val;
}

