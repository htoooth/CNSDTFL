#ifndef CIMG_H_INCLUDE

#define CIMG_H_INCLUDE

#include "fmts.cimgdataset.h"
#include "fmts.cimgrasterband.h"

namespace cimg
{

CIMGDataset * CIMGOpen ( const char * pszFileName );
void CIMGClose(CIMGDataset * poCIMGDataset);

}

#endif