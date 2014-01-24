#include "geoalgorithm.cvct2gdal.h"
#include "port.debug.h"

int CVCTAlgorithm::Init( int argc, char ** argv )
{
	for ( int i = 1; i < argc; i++ )
	{
		if ( EQUAL(argv[i], "-layer") )
		{
			papszLayers = CSLAddString(papszLayers, argv[++i]);
		}

		else if ( EQUAL(argv[i], "-format") && i < argc - 1 )
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
		{
			Help();
		}
	}

	if ( pszDstFile == NULL
	        || strcmp(pszSrcFile, pszDstFile) == 0)
	{
		Help();
	}

	return GEO_ALGORITHM_RIGHT;
}

int CVCTAlgorithm::GetDstDriver()
{
	poDstDriver = GetVectorDriver( pszFormat );

	if (poDstDriver == NULL)
	{
		Help();
	}

	return GEO_ALGORITHM_RIGHT;
}

int CVCTAlgorithm::OpenSrcDS()
{
	poSrcDS = cvct::CVCTOpen(pszSrcFile);

	if (poSrcDS == NULL)
	{
		Help();
	}

	if (papszLayers == NULL)
	{
		for (int ilayer = 0; ilayer != poSrcDS->GetLayerCount(); ++ilayer)
		{
			papszLayers =
			    CSLAddString(papszLayers,
			                 poSrcDS->GetLayerByIndex(ilayer)->GetLayerCode().c_str());
		}
	}

	return GEO_ALGORITHM_RIGHT;
}

int CVCTAlgorithm::CloseSrcDS()
{
	cvct::CVCTClose(poSrcDS);
	return GEO_ALGORITHM_RIGHT;
}

int CVCTAlgorithm::CreateDstDS()
{
	poDstDS = VectorCreate(pszFormat, pszDstFile, NULL);

	if (poDstDS == NULL)
	{
		Help();
	}

	return GEO_ALGORITHM_RIGHT;
}

int CVCTAlgorithm::OpenDstDS()
{
	poDstDS = VectorOpen(pszDstFile, GA_Update);

	if (poDstDS == NULL)
	{
		return GEO_ALGORITHM_WRONG;
	}

	return GEO_ALGORITHM_RIGHT;
}

int CVCTAlgorithm::CloseDstDS()
{
	VectorClose(poDstDS);
	return GEO_ALGORITHM_RIGHT;
}

int CVCTAlgorithm::GetComputeDataSize( DatBlock & data )
{
	data.nOffX = 0;
	data.nXSize = 1;

	data.nOffY = 0;
	data.nYSize = poSrcDS->GetLayerCount();

	return GEO_ALGORITHM_RIGHT;
}

int CVCTAlgorithm::Compute( const DatBlock & data )
{
	long layer = data.nOffY;
	long nlayer = data.nYSize;
	for (long ilayer = layer; ilayer < layer + nlayer; ++ilayer)
	{
		cvct::VCTLayer * poVCTLayer = poSrcDS->GetLayerByIndex(ilayer);
        ///!!!注意投影不可以转换，1999年的标准没有做
		OGRLayer * poOGRLayer =
		    poDstDS->CreateLayer(poVCTLayer->GetLayerCode().c_str(),
		                         NULL,
		                         poVCTLayer->ToOGRGeoType());

		REQUIRE(poOGRLayer != NULL);
        
		poVCTLayer->ToOGRLayer(poOGRLayer);
	}


	return GEO_ALGORITHM_RIGHT;
}

long CVCTAlgorithm::GetBlockYSize()
{
	return nBlockXSize;
}

long CVCTAlgorithm::GetBlockXSize()
{
	return nBlockYSize;
}

int CVCTAlgorithm::Clean()
{
	VectorClean();
	return GEO_ALGORITHM_RIGHT;
}

void CVCTAlgorithm::Help()
{
	printf(
	    "Usage: cvct2gdal [[-layer <layer1>] ...]\n"
	    "                 [-format <format_name>]\n"
	    "                 -src <src_filename>\n"
	    "                 -dst <dst_filename>\n" );
	exit(1);
}

CVCTAlgorithm::CVCTAlgorithm()
	: GEOAlgorithm("cvct2gdal")
	, poSrcDS(NULL)
	, poDstDS(NULL)
	, poDstDriver(NULL)
	, pszSrcFile(NULL)
	, pszDstFile(NULL)
	, pszFormat("ESRI Shapefile")
	, papszLayers(NULL)
	, nBlockXSize(1)
	, nBlockYSize(1)
{

}

CVCTAlgorithm::~CVCTAlgorithm()
{
	CPLFree(papszLayers);
}
