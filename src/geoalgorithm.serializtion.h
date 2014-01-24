#ifndef MPI_MESSAGE_FEATURE_H_INCLUDE

#define MPI_MESSAGE_FEATURE_H_INCLUDE

#include "geoalgorithm.format.h"

//ÐòÀý»¯
const char * OGRFeat_PACK(OGRFeature * oFeat);
const char * OGRFeatDefn_PACK(OGRFeatureDefn * oFeatDefn);
const char * OGRGeometry_PACK(OGRGeometry * oGeometry);
const char * OGRField_PACK(OGRFeature * oFeat);
const char * OGRFieldDefn_PACK(OGRFieldDefn * oFielddefn);

//·´ÐòÀý»¯
OGRFeature * OGRFeat_UNPACK(const char * pszFeat);
OGRFeatureDefn * OGRFeatDefn_UNPACK(const char * pszFeatDefn);
OGRGeometry * OGRGeometry_UNPACK(const char * pszGeo);
OGRFeature * OGRField_UNPACK(OGRFeature * poFeat, const char * pszField);
OGRFieldDefn * OGRFieldDefn_UNPACK(const char * pszFieldDefn);

#endif