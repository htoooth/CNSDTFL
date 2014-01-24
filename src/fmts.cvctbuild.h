#ifndef CVCT_BUILD_H_INCLUDE
#define CVCT_BUILD_H_INCLUDE

#include "fmts.cvct.h"

namespace cvctbuild
{
using cvct::LinePtr;
using cvct::FilePos;
using std::vector;

class IndexBuilding;

///状态类的基类
class IndexBuilder
{
public:

	std::string name;

	IndexBuilder() 
        : name ( "Null" ) 
    {};

	IndexBuilder ( std::string str ) 
        : name ( str ) 
    {};

	void CurrentIndexer ( IndexBuilding * pTemp )
	{
		SetIndexer ( pTemp );
	};

	virtual void SetIndexer ( IndexBuilding * ps ) = 0;

	virtual IndexBuilder * Clone() = 0;
};

//处理headbegin
class HeadIndexBuilder: public IndexBuilder
{
public:

	cvct::VCTSpatialReference srs;

	HeadIndexBuilder()
        : IndexBuilder ( "HeadBegin" ) 
    {};

	virtual void SetIndexer ( IndexBuilding * ps );

	virtual HeadIndexBuilder * Clone();

};

//处理featurecodebegin
class FeatureCodeIndexBuilder: public IndexBuilder
{
public:
	FeatureCodeIndexBuilder() 
        : IndexBuilder ( "FeatureCodeBegin" ) 
    {};

	void SetIndexer ( IndexBuilding * ps );

	FeatureCodeIndexBuilder * Clone();
};

//处理tablestructurebegin
class TableStructureIndexBuilder: public IndexBuilder
{
public:

	TableStructureIndexBuilder() 
        : IndexBuilder ( "TableStructureBegin" )
    {};

	void SetIndexer ( IndexBuilding * ps );

	TableStructureIndexBuilder * Clone();
};

//处理pointbegin
class PointIndexBuilder: public IndexBuilder
{
public:

	PointIndexBuilder() 
        : IndexBuilder ( "PointBegin" ) 
    {};

	void SetIndexer ( IndexBuilding * ps );

	PointIndexBuilder * Clone();
};

//处理linebegin
class LineIndexBuilder: public IndexBuilder
{
public:

	LineIndexBuilder() 
        : IndexBuilder ( "LineBegin" ) 
    {};

	void SetIndexer ( IndexBuilding * ps );

	LineIndexBuilder * Clone();

};

//处理polygonbegin
class PolygonIndexBuilder: public IndexBuilder
{
public:

	PolygonIndexBuilder() 
        : IndexBuilder ( "PolygonBegin" )
    {};

	void SetIndexer ( IndexBuilding * ps );

	PolygonIndexBuilder * Clone();
};

//处理attributebegin
class AttributeIndexBuilder: public IndexBuilder
{
public:

	AttributeIndexBuilder() 
        : IndexBuilder ( "AttributeBegin" )
    {};

	void SetIndexer ( IndexBuilding * ps );

	AttributeIndexBuilder * Clone();
};

//处理一般的情况
class CommIndexBuilder: public IndexBuilder
{
public:

	CommIndexBuilder() 
        : IndexBuilder ( "Null" ) 
    {};

	CommIndexBuilder ( std::string str ) 
        : IndexBuilder ( str ) 
    {};

	void SetIndexer ( IndexBuilding * ps );

	CommIndexBuilder * Clone();
};

///接受状态的主体
class IndexBuilding
{
public:

	friend class CommIndexBuilder;

	IndexBuilding ( cvct::VCTDataSource * pods )
        : m_state ( new CommIndexBuilder() )
        , m_line ( NULL )
        , datasource ( pods )
        , m_indexerlist()
	{
		InitialIndexBuilderList();
	};

	void SetIndexer ( IndexBuilder * poindex )
	{
		RemoveIndexer();
		m_state = poindex;
	};

	void RemoveIndexer()
	{
		delete m_state;
	};

	void BuildingIndex( LinePtr cln)
	{
		m_line = cln;
		m_state->CurrentIndexer ( this );
	};

	char * GetCurrentLine()
	{
		return m_line->line;
	};

	FilePos GetStartPos()
	{
		return m_line->start_pos;
	};

	FilePos GetEndPos()
	{
		return m_line->end_pos;
	};

    ///智能指针，只能返回一次
	LinePtr GetLinePtr()
	{
		return m_line;
	};

	~IndexBuilding()
	{
		delete m_state ;
		datasource = NULL;
	};

    ///这个是索引
	cvct::VCTDataSource * datasource;

private:

	vector<IndexBuilder *> m_indexerlist;

	IndexBuilder * m_state;

	LinePtr m_line;

	void InitialIndexBuilderList();

	bool IsEmptyIndexBuildererList();
};

}

#endif