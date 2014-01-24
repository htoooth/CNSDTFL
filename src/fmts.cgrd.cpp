#include "fmts.cgrd.h"

cgrd::CGRDDataset * cgrd::CGRDOpen ( const char * pszFileName )
{
	GDALOpenInfo oInfo ( pszFileName, GA_ReadOnly );
	return cgrd::CGRDDataset::Open ( &oInfo );
}

void cgrd::CGRDClose( cgrd::CGRDDataset * poCGRDDataSet )
{
	delete poCGRDDataSet ;
}
