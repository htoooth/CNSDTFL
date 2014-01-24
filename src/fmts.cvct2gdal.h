#ifndef CVCT2OGR_H_INCLUDE
#define CVCT2OGR_H_INCLUDE

#include "fmts.cvct.h"
#include "fmts.cvctparse.h"

namespace cvct2gdal
{
using namespace cvct;

void CVCT2GDALInit(VCTDataSource * ds);

void CVCT2GDALLayer(VCTLayer * vlayer, OGRLayer * olayer);

OGRFeature * CVCT2GDALFeature ( VCTFeature * vctfeature, OGRFeatureDefn * ogrfeaturedefn);

OGRSpatialReference  CVCT2GDALSpatialReference ( VCTSpatialReference & vctsrs );

OGRGeometry * CVCT2GDALGeometry ( VCTGeometry & vctgeometry );

OGRFeatureDefn * CVCT2GDALFeatureDefn ( VCTFeatureDefn * vctfeaturedefn );

int CVCT2GDALWriteFields ( VCTFeature & vctfeature, OGRFeature & ogrfeature );

OGRFieldType CVCT2GDALFieldType ( FieldType ft );

OGRwkbGeometryType CVCT2GDALGeometryType ( GeoType gt );

}

#endif

