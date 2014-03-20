#ifndef CVCT_H_INCLUDE
#define CVCT_H_INCLUDE

#include "geoalgorithm.format.h"

#include <memory>
#include <map>
#include <vector>

namespace cvct
{
using std::string;
using std::map;
using std::vector;
using std::auto_ptr;
using std::multimap;

const int headsize = 7;
extern const char * vcthead[headsize];
extern const char * vctend[headsize];

typedef vsi_l_offset FilePos;

typedef VSILFILE VFile;

typedef enum {FT_INT, FT_DOUBLE, FT_STRING, FT_VARBIN, FT_DATE, FT_UNKNOWN} FieldType;

typedef enum {GT_POINT, GT_LINE, GT_POLYGON, GT_UNKNOWN} GeoType;

//每一个几何对象有一个标识码
typedef int BSM;

inline GeoType GetNameToGeoType ( const char * name )
{
	if ( EQUAL ( name, "Point" ) )
	{
		return GT_POINT;
	}

	else if ( EQUAL ( name, "Line" ) )
	{
		return GT_LINE;
	}

	else if ( EQUAL ( name, "Polygon" ) )
	{
		return GT_POLYGON;
	}

	else
	{
		return GT_UNKNOWN;
	}
};

inline FieldType GetNameToFieldType ( const char * name )
{
	if ( EQUAL ( name, "Integer" ) )
	{
		return FT_INT;
	}

	else if ( EQUAL ( name, "Char" ) )
	{
		return FT_STRING;
	}

	else if ( EQUAL ( name, "Float" ) )
	{
		return FT_DOUBLE;
	}

	else if ( EQUAL ( name, "Varbin" ) )
	{
		return FT_VARBIN;
	}

	else if ( EQUAL ( name, "Date" ) )
	{
		return FT_DATE;
	}

	else
	{
		return FT_UNKNOWN;
	}
};

/************************************************************************/
/* 读文件使用的行对象                                                                     */
/************************************************************************/
struct LineObj
{
	FilePos start_pos;

	FilePos end_pos;

	char * line;

	LineObj() 
        : line ( NULL)
        , start_pos ( 0 )
        , end_pos ( 0 ) 
    {};

	LineObj ( FilePos nstart, FilePos nend, char * newline ) 
        : start_pos ( nstart )
        , end_pos ( nend )
        , line ( CPLStrdup ( newline ) ) 
    {};

	LineObj ( const LineObj & obj ) 
        : line ( CPLStrdup ( obj.line ) )
        , start_pos ( obj.start_pos )
        , end_pos ( obj.end_pos ) 
    {};

	///申请和释放统一，用CPL库
	~LineObj()
	{
		CPLFree ( line );
	};

};

typedef auto_ptr<LineObj> LinePtr;

class VCTDataSource;

struct VCTFeature;

struct VCTSpatialReference
{
	FilePos start_pos;

	FilePos end_pos;

	OGRSpatialReference ToOGRSrs();

	VCTSpatialReference() 
        : start_pos ( 0 )
        , end_pos ( 0 ) 
    {};

	VCTSpatialReference ( const VCTSpatialReference & obj )
        : start_pos ( obj.start_pos )
        , end_pos ( obj.end_pos ) 
    {};

	void operator =(const VCTSpatialReference & roj)
	{
		start_pos = roj.start_pos;
		end_pos = roj.end_pos;
	};
};

struct VCTFieldDefn
{
	string name;

	FieldType type;

	int width;

	int precision;

	VCTFieldDefn() 
        : name()
        , type ( FT_UNKNOWN )
        , width ( 0 )
        , precision ( 0 ) 
    {};

	VCTFieldDefn ( char * nm, FieldType ty, int wh, int pn ) 
        : name ( nm )
        , type ( ty )
        , width ( wh )
        , precision ( pn ) 
    {};

};

struct VCTFeatureDefn
{
	string tablename;

	vector<VCTFieldDefn> fielddefnlist;

	VCTFeatureDefn ( char * tc )
        : tablename ( tc ) 
    {};

	VCTFeatureDefn() 
    {};

	VCTFeatureDefn ( const VCTFeatureDefn & obj );

	void operator() ( VCTFeatureDefn * obj );

	VCTFeatureDefn & operator=(const VCTFeatureDefn & roj);
    
};

class VCTLayer
{
public:

	VCTSpatialReference srs;

	GeoType geotype;

	VCTFeatureDefn featuredefn;

	VCTLayer() 
        : geotype ( GT_UNKNOWN ) 
    {};

	VCTLayer ( GeoType ty, char * ln, char * lc, char * tn );

	void SetDataSource(VCTDataSource * pds);

	string GetLayerCode();

	string GetLayerTableName();

	void SetLayerCode(string layerid);

	void SetLayerTableName(string tn);

	string GetLayerName();

	void SetLayerName(string name);

	BSM GetFeatureId(int ide);

	VCTFeature * GetFeatureById(int id);

	VCTFeature * GetFeatureByIndex(int ide);

	int GetFeatureCount();

	void AddFeature(VCTFeature * feat);

	void ToOGRLayer(OGRLayer * olayer);

    OGRSpatialReference ToOGRSpatialR();

	OGRwkbGeometryType ToOGRGeoType();

private:

	vector<BSM> featidlist;

	VCTDataSource * ds;

	string layername;

	string layercode;

	string tablename;

};

struct VCTGeometry
{
	/************************************************************************/
	/*这个主要是调用接口                                                      */
	/************************************************************************/
	//id和objectcode
	BSM id;

	string objectcode;

	GeoType geotype;

	FilePos start_pos;

	FilePos end_pos;

	/************************************************************************/
	/* 记录类型,以后会用,额外的功能                                          */
	/************************************************************************/
	int	   	      nParts;//有多少个部分

	int	    *     panPartStart;//每一个部分开始的结点坐标,从开始

	unsigned char uPartType;//每一个部分的类型

	int		      nVertices;//存放所有的结点的个数

//////////////////////////////////////////////////////////////////////////

	VCTGeometry() 
        : geotype ( GT_UNKNOWN )
        , objectcode()
        , start_pos ( 0 )
        , end_pos ( 0 )
        , nParts(0)
        , panPartStart(NULL)
        , uPartType(1)
        , nVertices(0) 
    {};

	VCTGeometry ( GeoType gt, string objid, FilePos sp, FilePos ep );

	VCTGeometry ( const VCTGeometry & obj );

	VCTGeometry & operator=(const VCTGeometry & obj);

	~VCTGeometry();

};

struct VCTFieldValue
{
	BSM id;

	string objectcode;

	FilePos start_pos;

	FilePos end_pos;

	VCTFieldValue() 
        : objectcode()
        , start_pos ( 0 )
        , end_pos ( 0 ) 
    {};

	VCTFieldValue ( string ob, FilePos sp, FilePos ep )
        : objectcode ( ob )
        , start_pos ( sp )
        , end_pos ( ep ) 
    {};

	VCTFieldValue ( const VCTFieldValue & obj ) 
        : objectcode ( obj.objectcode )
        , start_pos ( obj.start_pos )
        , end_pos ( obj.end_pos )
    {};

};

struct VCTFeature
{
	int id;

	string objectcode;

	GeoType geotype;

	VCTGeometry geometry;

	VCTFeatureDefn * featuredefn;

	VCTFieldValue  fields;

	VCTFeature() 
        : featuredefn ( NULL ) 
    {};

	VCTFeature ( const VCTFeature & obj );

	VCTFeature ( VCTFeatureDefn * obj ) 
        : featuredefn ( obj ) 
    {};

	VCTFeature ( GeoType gt ) 
        : geotype ( gt ) 
    {};

	~VCTFeature()
	{
		featuredefn = NULL;
	};

};

class VCTDataSource
{
public:

	VCTSpatialReference  srs;

	friend VCTDataSource * CVCTOpen(const char * pszFile);

	VCTLayer * GetLayerById(string layercode);

	VCTLayer * GetLayerByIndex(int ind)
	{
		return layerlist[ind];
	}

	string GetLayerTableName(int ind)
	{
		return GetLayerByIndex(ind)->GetLayerTableName();
	};

	string GetLayerCode(int ind)
	{
		return GetLayerByIndex(ind)->GetLayerCode();
	};

	VCTFeature * GetFeatureById(int id)
	{
		return featurelist.at(id - 1);
	};

	int GetFeatureCount()
	{
		return featurelist.size();
	};

	int GetLayerCount()
	{
		return layerlist.size();
	};

	void AddLayer(VCTLayer * layer)
	{
        layer->srs=srs;
		layerlist.push_back(layer);
	};

	void AddFeature(VCTFeature * feat)
	{
		featurelist.push_back(feat);
	};

	void SetFile(VFile * fp)
	{
		vctfp = fp;
	};

	VFile * GetFile()
	{
		return vctfp;
	};

	VCTDataSource(const char * pszFile);

	VCTDataSource()
        : vctfp(NULL) 
    {};

	~VCTDataSource();

private:

	void Build();

	vector<VCTLayer *> layerlist;

	//这里的vctfeature的数量多，因为线构成面
	//所以有的线不包括在能形成的多边形中
	vector<VCTFeature *> featurelist;

	VFile * vctfp;
};

VCTDataSource * CVCTOpen(const char * pszFile);

void CVCTClose(VCTDataSource * pds);

}

#endif
