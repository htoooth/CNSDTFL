#include <algorithm>
#include <vector>
#include <string>
#include <utility>

#include <cpl_string.h>

#include "fmts.cvctbuild.h"
#include "fmts.cvct.h"
#include "fmts.cvctindex.h"

using namespace cvctbuild;
using cvct::vctend;
using std::string;

/************************************************************************/
/* ¾²Ì¬±äÁ¿                                                              */
/************************************************************************/
//head
static cvctindex::HeadIndexBuilding * poHIndex =
    new cvctindex::HeadIndexBuilding(new cvctindex::HeadIndexer1());

//featurecode
static cvctindex::FeatureCodeIndexBuilding * poFIndex =
    new cvctindex::FeatureCodeIndexBuilding(new cvctindex::FeatureCodeIndexer1());

//tablestructure
static cvctindex::TableStructureIndexBuilding * poTIndex =
    new cvctindex::TableStructureIndexBuilding(new cvctindex::TableStructureIndexer1());

//point
static cvctindex::PointIndexBuilding * poPIndex =
    new cvctindex::PointIndexBuilding(new cvctindex::PointIndexer1());

//line
static cvctindex::LineIndexBuilding * poLIndex =
    new cvctindex::LineIndexBuilding(new cvctindex::LineIndexer1());

//polygon
static cvctindex::PolygonIndexBuilding * poPolyIndex =
    new cvctindex::PolygonIndexBuilding(new cvctindex::PolygonIndexer1());

//attribue
static cvctindex::AttributeIndexBuilding * poAIndex =
    new cvctindex::AttributeIndexBuilding(new cvctindex::AttributeIndexer1());


void cvctbuild::HeadIndexBuilder::SetIndexer ( IndexBuilding * ps )
{
	if ( EQUALN ( ps->GetCurrentLine(), vctend[0], strlen ( vctend[0] ) ) )
	{
		srs.end_pos = ps->GetStartPos();
		ps->datasource->srs = srs;

		ps->SetIndexer ( new CommIndexBuilder() );

		delete poHIndex;
		return;
	}

	else
	{
		poHIndex->BuildingIndex(ps);

		if (srs.start_pos == 0)
		{
			srs.start_pos = ps->GetStartPos();
		}
	}
}

cvctbuild::HeadIndexBuilder * cvctbuild::HeadIndexBuilder::Clone()
{
	return new HeadIndexBuilder ( *this );
}

void cvctbuild::FeatureCodeIndexBuilder::SetIndexer ( IndexBuilding * ps )
{
	if ( EQUALN ( ps->GetCurrentLine(), vctend[1], strlen ( vctend[1] ) ) )
	{
		ps->SetIndexer ( new CommIndexBuilder() );

		delete poFIndex;
		return;
	}

	else
	{
		poFIndex->BuildingIndex(ps);

		if (poFIndex->layer != NULL)
		{
			ps->datasource->AddLayer(poFIndex->layer);
		}
	}
}

cvctbuild::FeatureCodeIndexBuilder * cvctbuild::FeatureCodeIndexBuilder::Clone()
{
	return new FeatureCodeIndexBuilder ( *this );
}

void cvctbuild::TableStructureIndexBuilder::SetIndexer ( IndexBuilding * ps )
{
	if ( EQUALN ( ps->GetCurrentLine(), vctend[2], strlen ( vctend[2] ) ) )
	{

		ps->SetIndexer ( new CommIndexBuilder() );
		delete poTIndex;
		return;
	}

	else
	{
		poTIndex->BuildingIndex(ps);
	}
}

cvctbuild::TableStructureIndexBuilder * cvctbuild::TableStructureIndexBuilder::Clone()
{
	return new TableStructureIndexBuilder ( *this );
}

void cvctbuild::PointIndexBuilder::SetIndexer ( IndexBuilding * ps )
{
	if ( EQUALN ( ps->GetCurrentLine(), vctend[3], strlen ( vctend[3] ) ) )
	{
		ps->SetIndexer ( new CommIndexBuilder() );
		delete poPIndex;
		return;
	}

	else
	{
		poPIndex->BuildingIndex(ps);
	}
}

cvctbuild::PointIndexBuilder * cvctbuild::PointIndexBuilder::Clone()
{
	return new PointIndexBuilder ( *this );
}

void cvctbuild::LineIndexBuilder::SetIndexer ( IndexBuilding * ps )
{
	if ( EQUALN ( ps->GetCurrentLine(), vctend[4], strlen ( vctend[4] ) ) )
	{
		ps->SetIndexer ( new CommIndexBuilder() );
		delete poLIndex;
		return;
	}

	else
	{
		poLIndex->BuildingIndex(ps);
	}
}

cvctbuild::LineIndexBuilder * cvctbuild::LineIndexBuilder::Clone()
{
	return new LineIndexBuilder ( *this );
}

void cvctbuild::PolygonIndexBuilder::SetIndexer ( IndexBuilding * ps )
{
	if ( EQUALN ( ps->GetCurrentLine(), vctend[5], strlen ( vctend[5] ) ) )
	{
		ps->SetIndexer ( new CommIndexBuilder() );
		delete poPolyIndex;
		return;
	}

	else
	{
		poPolyIndex->BuildingIndex(ps);
	}
}

cvctbuild::PolygonIndexBuilder * cvctbuild::PolygonIndexBuilder::Clone()
{
	return new	PolygonIndexBuilder ( *this );
}

void cvctbuild::AttributeIndexBuilder::SetIndexer ( IndexBuilding * ps )
{
	if ( EQUALN ( ps->GetCurrentLine(), vctend[6], strlen ( vctend[6] ) ) )
	{
		ps->SetIndexer ( new CommIndexBuilder() );
		delete poAIndex;
		return;
	}

	else
	{
		poAIndex->BuildingIndex(ps);
	}
}

cvctbuild::AttributeIndexBuilder * cvctbuild::AttributeIndexBuilder::Clone()
{
	return new AttributeIndexBuilder ( *this );
}

struct Op_1
{
	Op_1 ( char * str ) : m_st ( string ( str ) ) {};
	bool operator() ( const IndexBuilder * obj )
	{
		return obj->name == m_st;
	};
private:
	string m_st;
};

void cvctbuild::CommIndexBuilder::SetIndexer ( IndexBuilding * ps )
{
	if (ps->IsEmptyIndexBuildererList())
	{
		return;
	}

	auto f = std::find_if ( ps->m_indexerlist.begin(),
	                        ps->m_indexerlist.end(), Op_1 ( ps->GetCurrentLine() ) );

	if ( f == ps->m_indexerlist.end() )
	{
		return;
	}

	ps->SetIndexer ((*f)->Clone());
	ps->m_indexerlist.erase ( f );
}

cvctbuild::CommIndexBuilder * cvctbuild::CommIndexBuilder::Clone()
{
	return new CommIndexBuilder ( *this );
}

void cvctbuild::IndexBuilding::InitialIndexBuilderList()
{
	m_indexerlist.push_back ( new HeadIndexBuilder() );
	m_indexerlist.push_back ( new FeatureCodeIndexBuilder() );
	m_indexerlist.push_back ( new TableStructureIndexBuilder() );
	m_indexerlist.push_back ( new PointIndexBuilder() );
	m_indexerlist.push_back ( new LineIndexBuilder() );
	m_indexerlist.push_back ( new PolygonIndexBuilder() );
	m_indexerlist.push_back ( new AttributeIndexBuilder() );
}

bool cvctbuild::IndexBuilding::IsEmptyIndexBuildererList()
{
	return m_indexerlist.empty();
}
