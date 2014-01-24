#ifndef CVCT_INDEX_H_INCLUDE
#define CVCT_INDEX_H_INCLUDE

#include "fmts.cvctbuild.h"

namespace cvctindex
{
using cvctbuild::IndexBuilding;
using std::string;
/************************************************************************/
/* 头文件构建                                                            */
/************************************************************************/
class HeadIndexBuilding;

class HeadIndexer
{
public:

	virtual void SetIndex ( HeadIndexBuilding * pc ) = 0;

	void RunIndexer ( HeadIndexBuilding * pc )
	{
		SetIndex(pc);
	};
};

class HeadIndexBuilding
{
	HeadIndexer * indexpart;
public:
	IndexBuilding * builder;

	HeadIndexBuilding ( HeadIndexer * pp ) : indexpart ( pp ) {};

	~HeadIndexBuilding()
	{
		builder = NULL;
		delete indexpart;
	};

	void SetIndexer ( HeadIndexer * pp )
	{
		RemoveIndexer();
		indexpart = pp;
	};

	void RemoveIndexer()
	{
		delete indexpart;
	};

	void BuildingIndex ( IndexBuilding * ps )
	{
		builder = ps;
		indexpart->RunIndexer ( this );
	};
};

class HeadIndexer1: public HeadIndexer
{
public:
	void SetIndex ( HeadIndexBuilding * pc );
};

/*************************************************************/
/************************图层信息构建***************************/
/*************************************************************/
class FeatureCodeIndexBuilding;

class FeatureCodeIndexer
{
public:
	virtual void SetIndex ( FeatureCodeIndexBuilding * pc ) = 0;

	void RunIndexer ( FeatureCodeIndexBuilding * pc )
	{
		SetIndex(pc);
	};
};

class FeatureCodeIndexBuilding
{
	FeatureCodeIndexer * indexpart;

public:

	cvct::VCTLayer * layer;

	IndexBuilding * builder;

	FeatureCodeIndexBuilding ( FeatureCodeIndexer * pp ) : indexpart ( pp ) {};

	void SetIndexer ( FeatureCodeIndexer * pp )
	{
		RemoveIndexer();
		indexpart = pp;
	};

	void RemoveIndexer()
	{
		delete indexpart;
	};

	void BuildingIndex ( IndexBuilding * ps )
	{
		builder = ps;
		indexpart->RunIndexer ( this );
	};

};

class FeatureCodeIndexer1: public FeatureCodeIndexer
{
public:
	void SetIndex ( FeatureCodeIndexBuilding * pc );
};
/*************************************************************/
/************************属性表的构建***************************/
/*************************************************************/
class TableStructureIndexBuilding;

class TableStructureIndexer
{
public:

	virtual void SetIndex ( TableStructureIndexBuilding * pc ) = 0;

	void RunIndexer ( TableStructureIndexBuilding * pc )
	{
		SetIndex(pc);
	};
};

class TableStructureIndexBuilding
{
	TableStructureIndexer * indexpart;
public:

	int nfield;

	cvct::VCTLayer * layer;

	cvct::VCTFeatureDefn featuredefn;

	IndexBuilding * builder;

	TableStructureIndexBuilding ( TableStructureIndexer * pp ) : indexpart ( pp ) {};

	void SetIndexer ( TableStructureIndexer * pp )
	{
		RemoveIndexer();
		indexpart = pp;
	};

	void RemoveIndexer()
	{
		delete indexpart;
	};

	void BuildingIndex ( IndexBuilding * ps )
	{
		builder = ps;
		indexpart->RunIndexer ( this );
	};

};

class TableStructureIndexer1: public TableStructureIndexer
{
public:

	void SetIndex ( TableStructureIndexBuilding * pc );
};

class TableStructureIndexer2: public TableStructureIndexer
{
public:

	void SetIndex ( TableStructureIndexBuilding * pc );
};

/*************************************************************/
/***********************点的状态变化，5行共5种变化***************/
/*************************************************************/
class PointIndexBuilding;

class PointIndexer
{
public:
	virtual void SetIndexer ( PointIndexBuilding * pc ) = 0;

	void RunIndexer ( PointIndexBuilding * pc );
};

class PointIndexBuilding
{
	PointIndexer * indexpart;

public:

	IndexBuilding * builder;

	cvct::VCTFeature * feature;

	cvct::VCTLayer  *  layer;

	cvct::VCTGeometry geometry;

	string layercode;

	string objectcode;

	PointIndexBuilding ( PointIndexer * pp ) :
		indexpart ( pp ), feature ( NULL ) {};

	void SetIndexer ( PointIndexer * pp )
	{
		RemoveIndexer();
		indexpart = pp;
	};

	void RemoveIndexer()
	{
		delete indexpart;
	};

	void BuildingIndex ( IndexBuilding * ps )
	{
		builder = ps;
		indexpart->RunIndexer ( this );
	};

};

class PointIndexer1: public PointIndexer
{
public:
	void SetIndexer ( PointIndexBuilding * pc );
};

class PointIndexer2: public PointIndexer
{
public:
	void SetIndexer ( PointIndexBuilding * pc );
};

class PointIndexer3: public PointIndexer
{
public:
	void SetIndexer ( PointIndexBuilding * pc );
};

class PointIndexer4: public PointIndexer
{
public:
	void SetIndexer ( PointIndexBuilding * pc );
};

class PointIndexer5: public PointIndexer
{
public:
	void SetIndexer ( PointIndexBuilding * pc );

};
/************************************************************************/
/*  线的状态，part1表示第一行，part2表示第二行，part3表示第三行               */
/************************************************************************/
class LineIndexBuilding;
class LineIndexer
{
public:
	virtual void SetIndexer ( LineIndexBuilding * pc ) = 0;

	void RunIndexer ( LineIndexBuilding * pc );
};

class LineIndexBuilding
{
	LineIndexer * indexpart;
public:

	string objectcode;

	string layercode;

	int npoint;

	IndexBuilding * builder;

	cvct::VCTFeature * feature;

	cvct::VCTLayer  * layer;

	cvct::VCTGeometry geometry;

	LineIndexBuilding ( LineIndexer * lp ) :
		indexpart ( lp ), feature ( NULL ) {};

	void SetIndexer ( LineIndexer * lp )
	{
		RemoveIndexer();
		indexpart = lp;
	};

	void RemoveIndexer()
	{
		delete indexpart;
	};

	void BuildingIndex ( IndexBuilding * ps )
	{
		builder = ps;
		indexpart->RunIndexer ( this );
	};

};

class LineIndexer1: public LineIndexer
{
public:
	void SetIndexer ( LineIndexBuilding * pc );
};

class LineIndexer2: public LineIndexer
{
public:
	void SetIndexer ( LineIndexBuilding * pc );
};

class LineIndexer3: public LineIndexer
{
public:
	void SetIndexer ( LineIndexBuilding * pc );
};

class LineIndexer4: public LineIndexer
{
public:
	void SetIndexer ( LineIndexBuilding * pc );
};

class LineIndexer5: public LineIndexer
{
public:
	void SetIndexer ( LineIndexBuilding * pc );
};

class LineIndexer6: public LineIndexer
{
public:
	void SetIndexer ( LineIndexBuilding * pc );
};
/*************************************************************/
/**********************面状态**********************************/
/*************************************************************/
class PolygonIndexBuilding;
class PolygonIndexer
{
public:

	virtual void SetIndexer ( PolygonIndexBuilding * pc ) = 0;

	void RunIndexer ( PolygonIndexBuilding * pc );
};

class PolygonIndexBuilding
{
	PolygonIndexer * indexpart;
public:

	string objectcode;

	string layercode;

	int nline;

	IndexBuilding * builder;

	cvct::VCTFeature * feature;

	cvct::VCTLayer  * layer;

	cvct::VCTGeometry geometry;

	PolygonIndexBuilding ( PolygonIndexer * lp ) :
		indexpart ( lp ), feature (NULL ) {};

	void SetIndexer ( PolygonIndexer * lp )
	{
		RemoveIndexer();
		indexpart = lp;
	};

	void RemoveIndexer()
	{
		delete indexpart;
	};

	void BuildingIndex ( IndexBuilding * ps )
	{
		builder = ps;
		indexpart->RunIndexer ( this );
	};
};

class PolygonIndexer1: public PolygonIndexer
{
public:
	void SetIndexer ( PolygonIndexBuilding * pc );
};

class PolygonIndexer2: public PolygonIndexer
{
public:
	void SetIndexer ( PolygonIndexBuilding * pc );
};

class PolygonIndexer3: public PolygonIndexer
{
public:
	void SetIndexer ( PolygonIndexBuilding * pc );
};

class PolygonIndexer4: public PolygonIndexer
{
public:
	void SetIndexer ( PolygonIndexBuilding * pc );
};

class PolygonIndexer5: public PolygonIndexer
{
public:
	void SetIndexer ( PolygonIndexBuilding * pc );
};

class PolygonIndexer6: public PolygonIndexer
{
public:
	void SetIndexer ( PolygonIndexBuilding * pc );
};

/*************************************************************/
/************************属性状态开始***************************/
/*************************************************************/
class AttributeIndexBuilding;
class AttributeIndexer
{
public:
	virtual void SetIndexer ( AttributeIndexBuilding * pc ) = 0;

	void RunIndexer ( AttributeIndexBuilding * pc );
};

class AttributeIndexBuilding
{
	AttributeIndexer * indexpart;
public:
	string objectcode;

	string layercode;

	IndexBuilding * builder;

	cvct::VCTLayer * layer;

	AttributeIndexBuilding ( AttributeIndexer * ap ) : indexpart ( ap ), builder ( NULL ) {};

	void SetIndexer ( AttributeIndexer * ap )
	{
		RemoveIndexer();
		indexpart = ap;
	};

	void RemoveIndexer()
	{
		delete indexpart;
	};

	void BuildingIndex ( IndexBuilding * ps )
	{
		builder = ps;
		indexpart->RunIndexer ( this );
	};
};

class AttributeIndexer1: public AttributeIndexer
{
public:
	void SetIndexer ( AttributeIndexBuilding * pc );
};

class AttributeIndexer2: public AttributeIndexer
{
public:
	void SetIndexer ( AttributeIndexBuilding * pc );
};
}
#endif