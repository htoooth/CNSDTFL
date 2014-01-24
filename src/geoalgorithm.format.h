#ifndef GEO_ALGORITHM_FORMAT_H_INCLUDE

#define GEO_ALGORITHM_FORMAT_H_INCLUDE

#include <cpl_string.h>
#include <cpl_conv.h>
#include <cpl_port.h>
#include <cpl_vsi.h>

#include <ogr_api.h>
#include <ogr_feature.h>
#include <ogr_spatialref.h>
#include <ogrsf_frmts.h>

#include <gdal.h>
#include <gdal_priv.h>
#include <gdal_pam.h>
#include <vrtdataset.h>

///注册所有的格式，驱动包括矢量和栅格
void RegisterAll();

void RegisterVector();

void RegisterRaster();

///获得矢量数据驱动
OGRSFDriver * GetVectorDriver(const char * pszFormat);

///获得栅格数据驱动
GDALDriver * GetRasterDriver(const char * pszFormat );

///打开矢量数据集
OGRDataSource * VectorOpen(const char * pszFilename,
                           GDALAccess eAccess);

///打开栅格数据集
GDALDataset * RasterOpen(const char * pszFilename,
                         GDALAccess eAccess);

///创建矢量数据集
OGRDataSource * VectorCreate(const char * pszFormat,
                             const char * pszFilename,
                             char ** papszOptions = NULL);

///创建栅格数据集
GDALDataset * RasterCreate(const char * pszFormat,
                           const char * pszFilename,
                           char ** papszOptions = NULL);

///创建栅格数据集元数据
char ** SetCreateMetaData(int xsize, int ysize,
                          int band, GDALDataType eDataType);

///关闭矢量数据集
void VectorClose(OGRDataSource * poDS);

///关闭栅格数据集
void RasterClose(GDALDataset * poDS);

///清除矢量数据信息
void VectorClean();

///清除栅格数据信息
void RasterClean();

#endif