#include <cpl_string.h>

#include "fmts.cvctbuild.h"
#include "fmts.cvctindex.h"
#include "fmts.cvct.h"
#include "port.debug.h"

using namespace cvctindex;
void cvctindex::HeadIndexer1::SetIndex( HeadIndexBuilding * pc )
{
	//BUG(pc->builder->GetCurrentLine());
}

void cvctindex::FeatureCodeIndexer1::SetIndex( FeatureCodeIndexBuilding * pc )
{
	char ** papszFeatureCode =
	    CSLTokenizeString2 ( pc->builder->GetCurrentLine(), ",", CSLT_ALLOWEMPTYTOKENS );
	cvct::GeoType gt = cvct::GetNameToGeoType ( papszFeatureCode[2] );

	if ( gt == cvct::GT_UNKNOWN )
	{
		pc->layer = NULL;
		return;
	}

	pc->layer = new cvct::VCTLayer();

	pc->layer->geotype = gt;
	pc->layer->SetLayerCode(papszFeatureCode[0]);
	pc->layer->SetLayerName(papszFeatureCode[1]);
	pc->layer->SetLayerTableName(papszFeatureCode[6]);
	pc->layer->SetDataSource(pc->builder->datasource);
	CSLDestroy(papszFeatureCode);
}

void cvctindex::TableStructureIndexer1::SetIndex( TableStructureIndexBuilding * pc )
{
	char ** tablename =
	    CSLTokenizeString2 ( pc->builder->GetCurrentLine(), ",", CSLT_ALLOWEMPTYTOKENS );

	pc->featuredefn.tablename = tablename[0];
	pc->nfield = atoi(tablename[1]);

	for (auto ilayer = 0; ilayer != pc->builder->datasource->GetLayerCount(); ++ilayer)
	{
		if (pc->builder->datasource->GetLayerByIndex(ilayer)->GetLayerTableName() == pc->featuredefn.tablename)
		{
			pc->layer = pc->builder->datasource->GetLayerByIndex(ilayer);
			break;
		}

		else
		{
			pc->layer = NULL;
		}
	}

	pc->SetIndexer(new TableStructureIndexer2());
	CSLDestroy(tablename);
}

void cvctindex::TableStructureIndexer2::SetIndex( TableStructureIndexBuilding * pc )
{
	char ** tabledefn =
	    CSLTokenizeString2(pc->builder->GetCurrentLine(), ",", CSLT_ALLOWEMPTYTOKENS);
	tabledefn = CSLAddString(tabledefn, "0");
	tabledefn = CSLAddString(tabledefn, "0");

	pc->featuredefn.fielddefnlist.push_back(cvct::VCTFieldDefn(
	        tabledefn[0], cvct::GetNameToFieldType(tabledefn[1]),
	        atoi(tabledefn[2]), atoi(tabledefn[3])) );
	pc->nfield--;

	if (pc->nfield == 0)
	{
		pc->SetIndexer(new TableStructureIndexer1);

		if (pc->layer != NULL)
		{
			pc->layer->featuredefn = pc->featuredefn;
		}

		pc->featuredefn.fielddefnlist.clear();
	}

	CSLDestroy(tabledefn);
}

void cvctindex::PointIndexer::RunIndexer( PointIndexBuilding * pc )
{
	SetIndexer(pc);
}

void cvctindex::PointIndexer1::SetIndexer( PointIndexBuilding * pc )
{
	pc->feature = new cvct::VCTFeature(cvct::GT_POINT);
	pc->feature->objectcode = string(pc->builder->GetCurrentLine());
	pc->objectcode = string(pc->builder->GetCurrentLine());
	pc->feature->id = atoi(pc->builder->GetCurrentLine());

	pc->geometry.objectcode = pc->objectcode;
	pc->geometry.geotype = cvct::GT_POINT;

	BUG(pc->geometry.objectcode);

	pc->SetIndexer(new PointIndexer2());
}

void cvctindex::PointIndexer2::SetIndexer( PointIndexBuilding * pc )
{
	pc->layercode = string(pc->builder->GetCurrentLine());
	pc->layer = pc->builder->datasource->GetLayerById(pc->layercode);
	pc->SetIndexer(new PointIndexer3());
}

void cvctindex::PointIndexer3::SetIndexer( PointIndexBuilding * pc )
{
	pc->SetIndexer(new PointIndexer4());
	pc->feature->featuredefn = &pc->layer->featuredefn;
}

void cvctindex::PointIndexer4::SetIndexer( PointIndexBuilding * pc )
{
	pc->geometry.nParts = 1;
	pc->geometry.nVertices = atoi(pc->builder->GetCurrentLine());
	pc->geometry.uPartType = 1; //这里的1是默认值
	pc->geometry.panPartStart = new int(0);

	pc->SetIndexer(new PointIndexer5());
}

void cvctindex::PointIndexer5::SetIndexer(PointIndexBuilding * pc)
{
	pc->geometry.start_pos = pc->builder->GetStartPos();
	pc->geometry.end_pos = pc->builder->GetEndPos();
	pc->geometry.objectcode = pc->objectcode;
	pc->geometry.id = pc->feature->id;

	pc->feature->geometry = pc->geometry;

	pc->layer->AddFeature(pc->feature);

	pc->layer = NULL;
	pc->feature = NULL;

	pc->SetIndexer(new PointIndexer1());
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
void cvctindex::LineIndexer::RunIndexer( LineIndexBuilding * pc )
{
	SetIndexer(pc);
}

void cvctindex::LineIndexer1::SetIndexer(LineIndexBuilding * pc)
{
	pc->feature = new cvct::VCTFeature(cvct::GT_LINE);
	pc->objectcode = string(pc->builder->GetCurrentLine());
	pc->feature->objectcode = pc->objectcode;

	pc->geometry.objectcode = pc->objectcode;
	pc->geometry.geotype = cvct::GT_LINE;

	BUG(pc->geometry.objectcode);

	pc->feature->id = atoi(pc->builder->GetCurrentLine());
	pc->geometry.id = pc->feature->id;

	pc->SetIndexer(new LineIndexer2());
}

void cvctindex::LineIndexer2::SetIndexer(LineIndexBuilding * pc)
{
	pc->layercode = string(pc->builder->GetCurrentLine());
	pc->layer = pc->builder->datasource->GetLayerById(pc->layercode);
	pc->SetIndexer(new LineIndexer3());
}

void cvctindex::LineIndexer3::SetIndexer(LineIndexBuilding * pc)
{
	pc->feature->featuredefn = &pc->layer->featuredefn;
	pc->SetIndexer(new LineIndexer4());
}

void cvctindex::LineIndexer4::SetIndexer(LineIndexBuilding * pc)
{
	pc->geometry.nParts = 1;
	pc->geometry.uPartType = 1;
	pc->geometry.panPartStart = new int(0);

	pc->SetIndexer(new LineIndexer5());
}

void cvctindex::LineIndexer5::SetIndexer(LineIndexBuilding * pc)
{
	pc->npoint = atoi(pc->builder->GetCurrentLine());
	pc->geometry.nVertices = pc->npoint;
	pc->geometry.start_pos = pc->builder->GetEndPos(); //获得开始位置
	pc->SetIndexer(new LineIndexer6());
}

void cvctindex::LineIndexer6::SetIndexer(LineIndexBuilding * pc)
{
	if (!(--pc->npoint))
	{
		pc->geometry.end_pos = pc->builder->GetEndPos(); //获得结束位置
		pc->feature->geometry = pc->geometry;
		pc->layer->AddFeature(pc->feature);
		pc->SetIndexer(new LineIndexer1());

		pc->feature = NULL;
		pc->layer = NULL;
	}
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
void cvctindex::PolygonIndexer::RunIndexer( PolygonIndexBuilding * pc )
{
	SetIndexer(pc);
}

void cvctindex::PolygonIndexer1::SetIndexer( PolygonIndexBuilding * pc )
{
	pc->feature = new cvct::VCTFeature(cvct::GT_POLYGON);
	pc->objectcode = string(pc->builder->GetCurrentLine());

	pc->feature->objectcode = pc->objectcode;
	pc->geometry.objectcode = pc->objectcode;
	pc->geometry.geotype = cvct::GT_POLYGON;

	BUG(pc->geometry.objectcode);

	//new
	pc->feature->id = atoi(pc->builder->GetCurrentLine());
	pc->geometry.id = pc->feature->id;

	pc->SetIndexer(new	PolygonIndexer2());
}

void cvctindex::PolygonIndexer2::SetIndexer( PolygonIndexBuilding * pc )
{
	pc->layercode = string(pc->builder->GetCurrentLine());
	pc->layer = pc->builder->datasource->GetLayerById(pc->layercode);
	pc->SetIndexer(new PolygonIndexer3());
}

void cvctindex::PolygonIndexer3::SetIndexer( PolygonIndexBuilding * pc )
{
	pc->feature->featuredefn = &pc->layer->featuredefn;
	pc->SetIndexer(new PolygonIndexer4());
}

void cvctindex::PolygonIndexer4::SetIndexer( PolygonIndexBuilding * pc )
{
	pc->SetIndexer(new PolygonIndexer5());
	pc->geometry.uPartType = 0; //表示由线或者面构成
}

void cvctindex::PolygonIndexer5::SetIndexer( PolygonIndexBuilding * pc )
{
	pc->geometry.nVertices = atoi(pc->builder->GetCurrentLine());
	pc->geometry.nParts = 1;
	pc->geometry.panPartStart = new int(0);
	pc->nline = static_cast<int> (ceil(pc->geometry.nVertices / 8.0));

	pc->geometry.start_pos = pc->builder->GetEndPos(); //获得开始位置

	pc->SetIndexer(new PolygonIndexer6());
}

void cvctindex::PolygonIndexer6::SetIndexer( PolygonIndexBuilding * pc )
{
	char ** poLine =
	    CSLTokenizeString2 ( pc->builder->GetCurrentLine(), " \n\r\t,", 0 );
	int items = -1;
	int n = 0;

	while ((items = CSLFindString(poLine + n, "0") ) != -1)
	{
		n += items;
		n++;

		int * it = new int[pc->geometry.nParts + 1];
		memcpy(it, pc->geometry.panPartStart, sizeof(int)*pc->geometry.nParts);
		it[pc->geometry.nParts] = n + (pc->geometry.nParts - 1) * 8;
		delete pc->geometry.panPartStart;
		pc->geometry.panPartStart = it;
		pc->geometry.nParts++;
	}

	CSLDestroy(poLine);

	if (!(--pc->nline))
	{
		pc->geometry.end_pos = pc->builder->GetEndPos(); //获得结束位置
		pc->feature->geometry = pc->geometry;
		pc->layer->AddFeature(pc->feature);
		pc->SetIndexer(new PolygonIndexer1());
		pc->feature = NULL;
		pc->layer = NULL;
	}
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
void cvctindex::AttributeIndexer::RunIndexer( AttributeIndexBuilding * pc )
{
	SetIndexer(pc);
}

void cvctindex::AttributeIndexer1::SetIndexer( AttributeIndexBuilding * pc )
{
	string tablename(pc->builder->GetCurrentLine());

	for (auto ilayer = 0; ilayer != pc->builder->datasource->GetLayerCount(); ++ilayer)
	{
		if (pc->builder->datasource->GetLayerTableName(ilayer) == tablename)
		{
			pc->layer = pc->builder->datasource->GetLayerByIndex(ilayer);
			pc->SetIndexer(new AttributeIndexer2());
			break;
		}

		else
		{
			pc->layer = NULL;
		}
	}
}

void cvctindex::AttributeIndexer2::SetIndexer( AttributeIndexBuilding * pc )
{
	if (EQUAL(pc->builder->GetCurrentLine(), "TableEnd"))
	{
		pc->SetIndexer(new AttributeIndexer1());
		return ;
	}

	if (pc->layer == NULL)
	{
		return;
	}

	//find objectcode
	string temp(pc->builder->GetCurrentLine());
	auto pos = temp.find(",");
	string objectcode = temp.substr(0, pos);
	auto id = atoi(objectcode.c_str());

	cvct::VCTFieldValue field(objectcode,
	                          pc->builder->GetStartPos(),
	                          pc->builder->GetEndPos());
	pc->layer->GetFeatureById(id)->fields = field;
}

