#ifndef CGRD_H_INCLUDE
#define CGRD_H_INCLUDE

#include "geoalgorithm.format.h"

#include "port.debug.h"

#include "fmts.cgrddataset.h"
#include "fmts.cgrdrasterband.h"

namespace cgrd
{

CGRDDataset * CGRDOpen ( const char * pszFileName );

void CGRDClose(CGRDDataset * poCGRDDataSet);

}
#endif