#include "geoalgorithm.h"
#include <stdio.h>

int GEOAlgorithm::Init( int argc, char ** argv )
{
	return GEO_ALGORITHM_WRONG;
}

int GEOAlgorithm::GetSrcDriver()
{
	return GEO_ALGORITHM_RIGHT;
}

int GEOAlgorithm::GetDstDriver()
{
	return GEO_ALGORITHM_WRONG;
}

int GEOAlgorithm::OpenSrcDS()
{
	return GEO_ALGORITHM_WRONG;
}

int GEOAlgorithm::CloseSrcDS()
{
	return GEO_ALGORITHM_WRONG;
}

int GEOAlgorithm::CreateDstDS()
{
	return GEO_ALGORITHM_WRONG;
}

int GEOAlgorithm::OpenDstDS()
{
	return GEO_ALGORITHM_WRONG;
}

int GEOAlgorithm::CreateOrOpenDstDS()
{
	int nErr = GEO_ALGORITHM_WRONG;

	if (nErr = OpenDstDS())
	{
		nErr = CreateDstDS();
	}

	return nErr;
}

int GEOAlgorithm::CloseDstDS()
{
	return GEO_ALGORITHM_WRONG;
}

int GEOAlgorithm::GetComputeDataSize( DatBlock & data )
{
	return GEO_ALGORITHM_WRONG;
}

int GEOAlgorithm::Compute(const DatBlock & data)
{
	return GEO_ALGORITHM_WRONG;
}

int GEOAlgorithm::Clean()
{
	return GEO_ALGORITHM_WRONG;
}

int GEOAlgorithm::Test( int argc, char ** argv)
{
	printf("============================\n");
	printf("Test result:\n");

	if (Init(argc, argv))
	{
		printf("Init:               WRONG\n");
	}

	else
	{
		printf("Init:               OK\n");
	}

	if (GetSrcDriver())
	{
		printf("GetSrcDriver:       WRONG\n");
	}

	else
	{
		printf("GetSrcDriver:       OK\n");
	}

	if (GetDstDriver())
	{
		printf("GetDstDriver:       WRONG\n");
	}

	else
	{
		printf("GetDstDriver:       OK\n");
	}

	if (OpenSrcDS())
	{
		printf("OpenSrcDS:          WRONG\n");
	}

	else
	{
		printf("OpenSrcDS:          OK\n");
	}

	if (CreateOrOpenDstDS())
	{
		printf("CreateOrOpenDstDS:  WRONG\n");
	}

	else
	{
		printf("CreateOrOpenDstDS:  OK\n");
	}

	CloseDstDS();
	OpenDstDS();

	DatBlock pTemp;

	if (GetComputeDataSize(pTemp))
	{
		printf("GetComputeDataSize: WRONG\n");
	}

	else
	{
		printf("GetComputeDataSize: OK\n");
	}

	if (Compute(pTemp))
	{
		printf("Compute:            WRONG\n");
	}

	else
	{
		printf("Compute:            OK\n");
	}

	if (CloseSrcDS())
	{
		printf("CloseSrcDS:         WRONG\n");
	}

	else
	{
		printf("CloseSrcDS:         OK\n");
	}

	if (CloseDstDS())
	{
		printf("CloseDstDS:         WRONG\n");
	}

	else
	{
		printf("CloseDstDS:         OK\n");
	}

	if (Clean())
	{
		printf("Clean:              WRONG\n");
	}

	else
	{
		printf("Clean:              OK\n");
	}

	printf("============================\n");

	return GEO_ALGORITHM_RIGHT;
}

GEOAlgorithm::GEOAlgorithm( const char * pszNewName)
	: m_algname(pszNewName)
{

}

void GEOAlgorithm::Help()
{
	printf("NONE\n");
}

GEOAlgorithm::~GEOAlgorithm()
{

}

long GEOAlgorithm::GetBlockXSize()
{
	return NULL;
}

long GEOAlgorithm::GetBlockYSize()
{
	return NULL;
}

