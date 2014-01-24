#include "fmts.cgrdrasterband.h"
#include "fmts.cgrddataset.h"

using cgrd::CGRDDataset;

cgrd::CGRDRasterBand::CGRDRasterBand ( CGRDDataset * poDS, int nDataStart,
                                       GDALDataType eTypeIn )
{
	this->poDS = poDS;

	nBand = 1;

	///设置图象大小
	nRasterXSize = poDS->GetRasterXSize();
	nRasterYSize = poDS->GetRasterYSize();

	eDataType = eTypeIn;

	///设置块的大小
	nBlockXSize = poDS->GetRasterXSize();
	nBlockYSize = 1;

	SetNoDataValue ( poDS->dfNoDataValue );

	/************************************************************************/
	/* initalize panLineOffset                                              */
	/************************************************************************/

	panLineOffset = ( GUIntBig * ) VSICalloc ( poDS->GetRasterYSize(), sizeof ( GUIntBig ) );

	if ( panLineOffset == NULL )
	{
		CPLError ( CE_Failure, CPLE_OutOfMemory,
		           "[ERROR]CGRDRasterBand::CDEMRasterBand : Out of memory (nRasterYSize = %d)",
		           poDS->nRasterYSize );
		return;
	}

	for (int i = 0; i < poDS->GetRasterYSize(); ++i)
	{
		panLineOffset[i] = 0;
	}

	panLineOffset[0] = nDataStart;
}

cgrd::CGRDRasterBand::~CGRDRasterBand()
{
	CPLFree ( panLineOffset );
}

CPLErr cgrd::CGRDRasterBand::ReadBlock ( int nBlockXOff, int nBlockYOff, void * pImage )
{
	if ( panLineOffset[nBlockYOff] == 0 )
	{
		for ( int iPrevLine = 1; iPrevLine <= nBlockYOff; iPrevLine++ )
			if ( panLineOffset[iPrevLine] == 0 )
			{
				///返回数据上一行的结尾的数据指针位置
				ReadBlock ( nBlockXOff, iPrevLine - 1, NULL );
			}
	}

	if ( panLineOffset[nBlockYOff] == 0 )
	{
		return CE_Failure;
	}

	if ( poDS->Seek ( panLineOffset[nBlockYOff] ) != 0 )
	{
		CPLError ( CE_Failure, CPLE_FileIO,
		           "[ERROR]Can't seek to offset %lu in input file to read data.",
		           ( long unsigned int ) panLineOffset[nBlockYOff] );
		return CE_Failure;
	}

	/************************************************************************/
	/* !!!Read data from source data source.                                */
	/*    Data size depend on nBlockXSize and nBlockYSize.                  */
	/************************************************************************/
	///由字符组成数字，将nBlockXOff*nBlockYOff大小的数据组成

	for ( int iPixel = 0; iPixel < poDS->BlockXSize(); ++iPixel)
	{
		char szBuffer[500];
		char chNext;
		int  iTokenChar = 0;

		/************************************************************************/
		/* skip any pre-white space.                                            */
		/************************************************************************/

		do
		{
			chNext = poDS->Getc();
		}
		while ( isspace ( ( unsigned char ) chNext ) );

		/************************************************************************/
		/* Read a TokenChar, such as 1212.3 or 1000.                            */
		/************************************************************************/

		while ( chNext != '\0' && !isspace ( ( unsigned char ) chNext ) )
		{
			if ( iTokenChar == sizeof ( szBuffer ) - 2 )
			{
				CPLError ( CE_Failure, CPLE_FileIO,
				           "[ERROR]Token too long at scanline %d.",
				           nBlockYOff );
				return CE_Failure;
			}

			szBuffer[iTokenChar++] = chNext;
			chNext = poDS->Getc();
		}

		if ( chNext == '\0'
		        && ( iPixel != poDS->GetRasterXSize() - 1
		             || nBlockYOff != poDS->GetRasterYSize() - 1 ) )
		{
			CPLError ( CE_Failure, CPLE_FileIO,
			           "[ERROR]File short, can't read line %d.",
			           nBlockYOff );

			return CE_Failure;
		}

		/************************************************************************/
		/* One TokenChar.                                                       */
		/************************************************************************/

		szBuffer[iTokenChar] = '\0';

		BUG(szBuffer);
		BUG(iPixel);

		/************************************************************************/
		/* convert data.                                                        */
		/************************************************************************/

		if ( pImage != NULL )
		{
			///ascii -> double
			if ( eDataType == GDT_Float64 )
			{
				( ( double * ) pImage ) [iPixel] =
				    CPLAtofM ( szBuffer ) / poDS->GetnHZoom();
			}

			///ascii -> float
			///!!! double -> float, maybe have some questions.
			else if ( eDataType == GDT_Float32 )
			{
				( ( float * ) pImage ) [iPixel] =
				    ( ( float ) CPLAtofM ( szBuffer ) ) / poDS->GetnHZoom();
			}

			///ascii -> number
			else
			{
				( ( GInt32 * ) pImage ) [iPixel] =
				    ( ( GInt32 ) atoi ( szBuffer ) ) / poDS->GetnHZoom();
			}
		}
	}

	/************************************************************************/
	/* record start position of next block.                                 */
	/************************************************************************/

	if ( nBlockYOff < poDS->GetRasterYSize() - 1 )
	{
		panLineOffset[nBlockYOff + 1] = poDS->Tell();
	}

	return CE_None;
}

double cgrd::CGRDRasterBand::GetNoDataValue ( int * pbSuccess )
{
	*pbSuccess = this->bNoDataSet;
	return this->dfNoDataValue;
}

CPLErr cgrd::CGRDRasterBand::SetNoDataValue ( double dfNoData )
{
	this->bNoDataSet = TRUE;
	this->dfNoDataValue = dfNoData;

	return CE_None;
}

int cgrd::CGRDRasterBand::GetnBlockXSize()
{
	return nBlockXSize;
}

int cgrd::CGRDRasterBand::GetnBlockYsize()
{
	return nBlockYSize;
}

GDALDataType cgrd::CGRDRasterBand::GetRasterDataType()
{
	return eDataType;
}

int cgrd::CGRDRasterBand::RasterXSize() const
{
	return nRasterXSize;
}

void cgrd::CGRDRasterBand::RasterXSize( int val )
{
	nRasterXSize = val;
}

int cgrd::CGRDRasterBand::RasterYSize() const
{
	return nRasterYSize;
}

void cgrd::CGRDRasterBand::RasterYSize( int val )
{
	nRasterYSize = val;
}
