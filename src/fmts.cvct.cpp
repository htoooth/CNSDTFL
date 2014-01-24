#include "fmts.cvct.h"
#include "fmts.cvctbuild.h"
#include "fmts.cvct2gdal.h"
#include "port.debug.h"

using namespace cvct;

const char * cvct::vcthead[cvct::headsize] =
{
	"HeadBegin",
	"FeatureCodeBegin",
	"TableStructureBegin",
	"PointBegin",
	"LineBegin",
	"PolygonBegin",
	"AttributeBegin"
};

const char * cvct::vctend[cvct::headsize] =
{
	"HeadEnd",
	"FeatureCodeEnd",
	"TableStructureEnd",
	"PointEnd",
	"LineEnd",
	"PolygonEnd",
	"AttributeEnd"
};

static cvct::LinePtr ReadLine ( VFile * vsifile )
{
	vsi_l_offset start_pos = VSIFTellL ( vsifile );
	char * line_str = CPLStrdup ( CPLReadLineL ( vsifile ) );
	vsi_l_offset end_pos = VSIFTellL ( vsifile );
	return cvct::LinePtr ( new cvct::LineObj ( start_pos, end_pos, line_str ) );
}

cvct::VCTDataSource * cvct::CVCTOpen( const char * pszFile )
{
	auto ds = new cvct::VCTDataSource(pszFile);
	ds->Build();
	return ds;
}

void cvct::CVCTClose( VCTDataSource * pds )
{
	delete pds;
}

void cvct::VCTLayer::AddFeature( VCTFeature * feat )
{
	if (feat->geotype == geotype)
	{
		featidlist.push_back(feat->id);
		ds->AddFeature(feat);
	}

	else
	{
		ds->AddFeature(feat);
	}
}

cvct::VCTFeature * cvct::VCTLayer::GetFeatureById( int id )
{
	return ds->GetFeatureById(id);
}

void cvct::VCTLayer::ToOGRLayer( OGRLayer * olayer )
{
	cvct2gdal::CVCT2GDALLayer(this, olayer);
}

OGRwkbGeometryType cvct::VCTLayer::ToOGRGeoType()
{
	return cvct2gdal::CVCT2GDALGeometryType(geotype);
}

OGRSpatialReference cvct::VCTLayer::ToOGRSpatialR()
{
	return srs.ToOGRSrs();
}

void cvct::VCTLayer::SetDataSource( VCTDataSource * pds )
{
	ds = pds;
}

string cvct::VCTLayer::GetLayerCode()
{
	return layercode;
}

string cvct::VCTLayer::GetLayerTableName()
{
	return tablename;
}

void cvct::VCTLayer::SetLayerCode( string layerid )
{
	layercode = layerid;
}

void cvct::VCTLayer::SetLayerTableName( string tn )
{
	tablename = tn;
}

string cvct::VCTLayer::GetLayerName()
{
	return layername;
}

void cvct::VCTLayer::SetLayerName( string name )
{
	layername = name;
}

cvct::BSM cvct::VCTLayer::GetFeatureId( int ide )
{
	return featidlist[ide];
}

VCTFeature * cvct::VCTLayer::GetFeatureByIndex( int ide )
{
	return GetFeatureById(GetFeatureId(ide));
}

int cvct::VCTLayer::GetFeatureCount()
{
	return featidlist.size();
}

cvct::VCTLayer::VCTLayer( GeoType ty, char * ln, char * lc, char * tn ) 
    : geotype ( ty )
    , layername ( ln )
    , layercode ( lc )
    , tablename ( tn )
{}

cvct::VCTLayer * cvct::VCTDataSource::GetLayerById( string layercode )
{
	for (auto ilayer = 0; ilayer != GetLayerCount(); ++ilayer)
	{
		if (GetLayerCode(ilayer) == layercode)
		{
			return GetLayerByIndex(ilayer);
		}
	}

	return NULL;
}

cvct::VCTDataSource::~VCTDataSource()
{
	for (auto iter = featurelist.begin();
	        iter != featurelist.end(); ++iter)
	{
		delete *iter;
	}

	for (auto iter = layerlist.begin();
	        iter != layerlist.end(); ++iter)
	{
		delete *iter;
	}

	if (vctfp != NULL)
	{
		VSIFCloseL ( vctfp );
	}
}

cvct::VCTDataSource::VCTDataSource( const char * pszFile )
{
	vctfp = VSIFOpenL(pszFile, "r");
}

void cvct::VCTDataSource::Build()
{
	cvctbuild::IndexBuilding build(this);

	while ( !VSIFEofL ( vctfp) )
	{
		LinePtr ptr = ReadLine ( vctfp);
		build.BuildingIndex(ptr);
	}

	BUG("BUILD!");
	cvct2gdal::CVCT2GDALInit(this);
}

cvct::VCTFeature::VCTFeature( const VCTFeature & obj ) 
    : featuredefn ( obj.featuredefn )
    , objectcode ( obj.objectcode )
    , geometry ( obj.geometry )
    , fields ( obj.fields )
    , geotype ( obj.geotype )
{}

cvct::VCTGeometry::VCTGeometry( const VCTGeometry & obj ) 
    : objectcode ( obj.objectcode )
    , geotype ( obj.geotype )
    , start_pos ( obj.start_pos )
    , end_pos ( obj.end_pos )
    , nParts(obj.nParts)
    , uPartType(NULL)
    , panPartStart(NULL)
    , nVertices(obj.nVertices)
{
	panPartStart = new int[obj.nParts];
	uPartType = obj.uPartType;
	memcpy(panPartStart, obj.panPartStart, sizeof(int)*obj.nParts);
	id = obj.id;
}

cvct::VCTGeometry::VCTGeometry( GeoType gt, string objid, FilePos sp, FilePos ep ) 
    : geotype ( gt )
    , objectcode ( objid )
    , start_pos ( sp )
    , end_pos ( ep )
    , nParts(0)
    , uPartType(0)
    , panPartStart(NULL)
    , nVertices(0) 
{}

cvct::VCTGeometry::~VCTGeometry()
{
	if (panPartStart)
	{
		delete panPartStart;
	}
}

cvct::VCTGeometry & cvct::VCTGeometry::operator=( const VCTGeometry & obj )
{
	objectcode = obj.objectcode;
	geotype = obj.geotype;
	start_pos = obj.start_pos;
	end_pos = obj.end_pos;

	if (panPartStart != NULL)
	{
		delete panPartStart;
		panPartStart = NULL;
	}

	panPartStart = new int[obj.nParts];
	uPartType = obj.uPartType;
	memcpy(panPartStart, obj.panPartStart, sizeof(int)*obj.nParts);
	id = obj.id;

	return *this;
}

cvct::VCTFeatureDefn::VCTFeatureDefn( const VCTFeatureDefn & obj ) 
    : tablename ( obj.tablename )
    , fielddefnlist ( obj.fielddefnlist ) 
{}

void cvct::VCTFeatureDefn::operator()( VCTFeatureDefn * obj )
{
	tablename = obj->tablename;
	fielddefnlist = obj->fielddefnlist;
}

cvct::VCTFeatureDefn & cvct::VCTFeatureDefn::operator=( const VCTFeatureDefn & roj )
{
	tablename = roj.tablename;
	fielddefnlist = roj.fielddefnlist;
	return *this;
}

OGRSpatialReference cvct::VCTSpatialReference::ToOGRSrs()
{
	return cvct2gdal::CVCT2GDALSpatialReference(*this);
}
