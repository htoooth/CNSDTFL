#ifndef CSRS_H_INCLUDE

#define CSRS_H_INCLUDE

#include <string>

#include "geoalgorithm.format.h"
/************************************************************************/
/* 只支持GB 2007年                                                       */
/************************************************************************/
/***************************************************************/
/*              提取国家标准投影从附加信息或头文件中                */
/***************************************************************/
namespace csr   /*国家标准空间参考系统*/
{

using std::string;
/*************************************/
/*              地面控制点           **/
/*************************************/
typedef struct
{
	/* 控制点在图像上的行号 */
	long        ilGCPPixel;
	/* 控制点在图像上的列号 */
	long        ilGCPLine;
	/* 控制点的x坐标值 */
	double      dfGCPPixel;
	/* 控制点的y坐标值*/
	double      dfGCPLine;
	/* 图像x方向地面分辨率 */
	double      dfGCPX;
	/* 图像y方向地面分辨率 */
	double      dfGCPY;
} CSRS_GCP;/*地面控制点*/

/*************************************/
/*              参考椭球体           **/
/*************************************/
typedef struct
{
	string Type;/*参考椭球名*/
	double Semi_MajorAxis;/*长半轴*/
	double ReciprocalOfFlattening;/*扁率的倒数*/
} spheroid; /*参考椭球体*/

/*************************************/
/*              首子午线             **/
/*************************************/
typedef enum
{
    PM_GREENWICH,/*格林尼治*/
    PM_OTHER     /*其他*/
} prime_meridian_type; /*首子午线类型*/

typedef struct
{
	string FullName;/*首子午线名，默认格林尼治子午线*/
	double GreenwichLongitude;/*首子午线格林尼治经度*/
} prime_meridian_parameters; /*详细参数*/

typedef struct
{
	prime_meridian_type  Type;/*首子午线类型*/
	prime_meridian_parameters Parameters;/*详细参数*/
} prime_meridian; /*首子午线*/

/*************************************/
/*              投影                 **/
/*************************************/
typedef enum
{
    ZOME_SIZE_1,/*1.5度分带*/
    ZOME_SIZE_3,/*3度分带*/
    ZOME_SIZE_6,/*6度分带*/
    ZOME_SIZE_OTHER/*其他*/
} zone_type; /*投影带的类型*/

typedef struct
{
	zone_type Type;/*投影带的类型*/
	double Size;/*带宽*/
} zone; /*投影*/

typedef struct
{
	string FullName;/*类型*/
	struct
	{
		double    OiginLongitude;/*原点经度*/
		double    OiginLatitude;/*原点纬度*/
		double    StandardParallelOne;/*第一标准纬线*/
		double    StandardParallelTwo;/*第二标准纬线*/
		double    Azimuth;/*方位角*/
		double    ScaleFactor;/*比例因子*/
		double    FalseEasting;/*东偏*/
		double    FalseNorthing;/*北偏*/
		zone      ZoneSize;/*带宽*/
		int       ZoneNum;/*带号*/
	} Parameters; /*投影参数*/
} project; /*投影*/

/*************************************/
/*              高程基准系            */
/*************************************/
typedef enum
{
    DATUM_HUANGHAI1985,/*1985黄海*/
    DATUM_HUANGHAI1956,/*1956黄海*/
    DATUM_OTHER        /*其他*/
} datum_type; /*基准类型*/

typedef struct
{
	string FullName;/*全名*/
	double Dvalue;/*与1985高程基准的较差*/
} datum_parameters; /*高程基准详细参数*/

typedef struct
{
	datum_type  Type;/*类型*/
	datum_parameters Parameters;/*高程基准详细参数*/
} verticaldatum; /*高程基准*/

/*************************************/
/*              时间参照系            */
/*************************************/
typedef enum
{
    TEMPORAL_UTC,/*协调世界时*/
    TEMPORAL_OTHER/*其他时间*/
} temporal_type; /*类型*/

typedef struct
{
	string FullName;/*类型*/
	string Dvalue;/*与协调世界时的时差（+|-hhmm）*/
} temporal_parameters; /*时间参照系参数*/

typedef struct
{
	temporal_type   Type;/*类型*/
	temporal_parameters  Parameters;/*时间参照系参数*/
} temporal_reference_system; /*时间参照系*/

/*************************************/
/*              xyz单位              */
/*************************************/
typedef enum
{
    UN_M,   /*标准单位米*/
    UN_D,   /*标准单位度*/
    UN_O    /*其他单位*/
} unit_type_name; /*单位类型名*/

typedef enum
{
    UNIT_LENGTH,/*类型为长度单位，标准单位为米*/
    UNIT_ANGLE, /*类型为角度单位，标准单位为度*/
    UNIT_OTHER  /*其他类形*/
} unit_type; /*单位类型*/

typedef struct
{
	string    FullName;/*单位名称*/
	unit_type UnitType;/*单位类型*/
	double    UnitFactor;/*单位因子，与标准单位的比例因子，如千米为1000*/
} unit_parameters; /*单位详细情况*/

typedef struct
{
	unit_type_name  Type;  /*单位类型*/
	unit_parameters Parameters;/*单位详细情况*/
} xyz_unit; /*xyz单位*/

/*************************************/
/*              坐标系                */
/*************************************/
typedef enum
{
    COORDINATESYSTEM_C,/*笛卡尔坐标系*/
    COORDINATESYSTEM_D,/*大地坐标系*/
    COORDINATESYSTEM_P /*投影坐标系*/
} coordinate_system_type; /*坐标系类型*/

typedef struct
{
	coordinate_system_type CoordinateSystemType;/*坐标系类型*/
	xyz_unit XYUnit;/*xy的单位*/
	xyz_unit ZUnit;/*z的单位*/
	spheroid  Spheroid;/*参考椭球体*/
	prime_meridian PrimeMeridian;/*首子午线*/
	project Projection;/*投影*/
	temporal_reference_system TemporalReferenceSystem;/*时间参照系*/
	verticaldatum Datum;/*高程基准*/
} coordinate_system; /*坐标系*/

coordinate_system ExtractCSRFromAI ( char ** additonalinformation );

void CSR2WKT ( coordinate_system csr, char ** papszWKT );

OGRSpatialReference CSRS2OGRSRS ( coordinate_system cs );

}

#endif

