#ifndef CVCT_PARSE_H_INCLUDE

#define CVCT_PARSE_H_INCLUDE

#include "fmts.cvct.h"

namespace cvctparse
{

using cvct::VFile;

class GeometryPoint
{
public:
	double X;

	double Y;

	double Z;

	GeometryPoint() 
        : X ( 0.0 )
        , Y ( 0.0 )
        , Z ( 0.0 ) 
    {};

	GeometryPoint ( double x, double y, double z = 0 ) 
        : X ( x )
        , Y ( y )
        , Z ( z ) 
    {};

	GeometryPoint ( const GeometryPoint & obj ) 
        : X ( obj.X )
        , Y ( obj.Y )
        , Z ( obj.Z ) 
    {};

};

typedef std::vector<GeometryPoint> GeometryPart;
typedef std::vector<GeometryPart *> Geometry;
typedef std::vector<std::string> Fields;

void DestroyGeometry(Geometry * poGeometry);

class ParserIndex
{
protected:

	VFile * vctfile;

	cvct::VCTDataSource * vctindex;

public:

	ParserIndex ( VFile * pf, cvct::VCTDataSource * pds ) 
        : vctfile ( pf )
        , vctindex ( pds ) 
    {};

    //读出的数据最后多加了一个NULL,变成了字符串,大小比endpos-startpos大1
	size_t Read ( char ** pdata
	       , cvct::FilePos startpos
	       , cvct::FilePos endpos );

};

class SRSParser: public ParserIndex   //投影
{
public:

	SRSParser ( VFile * pf, cvct::VCTDataSource * pds ) 
        : ParserIndex ( pf, pds ) 
    {};

    OGRSpatialReference Parse(cvct::VCTSpatialReference &);
    
};

class GeoParser: public ParserIndex   //几何对象
{
public:

	GeoParser ( VFile * pf, cvct::VCTDataSource * pds ) 
        : ParserIndex ( pf, pds ) 
    {};

	virtual Geometry * Parse ( cvct::VCTGeometry & ) = 0;
};

class PointParser: public GeoParser   //点
{
public:

	PointParser ( VFile * pf, cvct::VCTDataSource * pds ) 
        : GeoParser ( pf, pds ) 
    {};

	Geometry * Parse ( cvct::VCTGeometry & );
};

class LineParser: public GeoParser   //线
{
public:

	LineParser ( VFile * pf, cvct::VCTDataSource * pds ) 
        : GeoParser ( pf, pds ) 
    {};

	Geometry * Parse ( cvct::VCTGeometry & );
};

class PolyParser: public GeoParser   //面
{
public:

	PolyParser ( VFile * pf, cvct::VCTDataSource * pds ) 
        : GeoParser ( pf, pds ) 
    {};

	Geometry * Parse ( cvct::VCTGeometry & );
};

class AttrParser: public ParserIndex   //属性
{
public:

	AttrParser ( VFile * pf, cvct::VCTDataSource * pds ) 
        : ParserIndex ( pf, pds ) 
    {};

	char ** Parse ( cvct::VCTFieldValue & );
};

}

#endif
