#include "fmts.cimg.h"

using namespace cimg;

cimg::CIMGDataset * cimg::CIMGOpen ( const char * pszFileName )
{
	GDALOpenInfo oInfo ( pszFileName, GA_ReadOnly );
	return CIMGDataset::Open ( &oInfo );
}

void cimg::CIMGClose( CIMGDataset * poCIMGDataset )
{
	delete poCIMGDataset;
}
