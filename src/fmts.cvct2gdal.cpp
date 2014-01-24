#include <ogr_feature.h>
#include <cpl_conv.h>
#include <ogrsf_frmts.h>

#include "fmts.cvct2gdal.h"
#include "fmts.cvct.h"
#include "port.debug.h"

using namespace std;

using namespace cvct;
using namespace cvct2gdal;
using namespace cvctparse;

static cvct::VCTDataSource * poVCTDataSource = NULL;
static cvct::VFile * poVCTFile = NULL;

OGRLinearRing * CreateLinearRing ( const GeometryPart & gPart )
{
	OGRLinearRing * poOGRRing = new OGRLinearRing();

	for ( auto ipart = gPart.begin(); ipart != gPart.end(); ++ipart )
	{
		poOGRRing->addPoint ( ipart->X, ipart->Y );
	}

	return poOGRRing;
}

OGRwkbGeometryType cvct2gdal::CVCT2GDALGeometryType ( GeoType VCTGeoType )
{
	if ( VCTGeoType == GT_POINT )
	{
		return wkbPoint;
	}

	else if ( VCTGeoType == GT_LINE )
	{
		return wkbLineString;
	}

	else if ( VCTGeoType == GT_POLYGON )
	{
		return wkbPolygon;
	}

	else
	{
		return wkbUnknown;
	}
}

OGRFieldType cvct2gdal::CVCT2GDALFieldType ( FieldType VCTFieldType )
{
	if ( VCTFieldType == FT_INT )
	{
		return ( OFTInteger );
	}

	else if ( VCTFieldType == FT_DOUBLE )
	{
		return ( OFTReal );
	}

	else if ( VCTFieldType == FT_STRING )
	{
		return ( OFTString );
	}

	else if ( VCTFieldType == FT_VARBIN )
	{
		return OFTString ;
	}

	else if ( VCTFieldType == FT_DATE )
	{
		return OFTString;
	}

	else
	{
		return OFTString;
	}
}

OGRFeature * cvct2gdal::CVCT2GDALFeature( VCTFeature * poVCTFeat, OGRFeatureDefn * poOGRFeatDefn )
{
	OGRFeature * poOGRFeat = new OGRFeature ( poOGRFeatDefn );
	OGRGeometry * poOGRGeometry = CVCT2GDALGeometry ( poVCTFeat->geometry );
	poOGRFeat->SetGeometryDirectly ( poOGRGeometry );
	CVCT2GDALWriteFields ( *poVCTFeat, *poOGRFeat );
	return poOGRFeat;
}

OGRSpatialReference cvct2gdal::CVCT2GDALSpatialReference ( VCTSpatialReference & vctsrs )
{
    SRSParser srs(poVCTFile,poVCTDataSource); 
	OGRSpatialReference  ogrsrs =srs.Parse(vctsrs);
	return ogrsrs;
}

OGRFeatureDefn * cvct2gdal::CVCT2GDALFeatureDefn ( VCTFeatureDefn * poVCTFeatDefn )
{
	OGRFeatureDefn * poOGRFeatDefn = new OGRFeatureDefn();

	for ( auto iVCTFieldDefn = poVCTFeatDefn->fielddefnlist.begin();
	        iVCTFieldDefn != poVCTFeatDefn->fielddefnlist.end(); ++iVCTFieldDefn )
	{
		OGRFieldDefn oOGRFieldDefn ( "", OFTInteger );
		oOGRFieldDefn.SetName ( iVCTFieldDefn->name.c_str() );
		oOGRFieldDefn.SetWidth ( iVCTFieldDefn->width );
		oOGRFieldDefn.SetPrecision ( iVCTFieldDefn->precision );
		oOGRFieldDefn.SetType ( CVCT2GDALFieldType ( iVCTFieldDefn->type ) );
		poOGRFeatDefn->AddFieldDefn ( &oOGRFieldDefn );
	}

	return poOGRFeatDefn;
}

int cvct2gdal::CVCT2GDALWriteFields ( VCTFeature & oVCTFeat, OGRFeature & oOGRFeat )
{
	AttrParser attr ( poVCTFile, poVCTDataSource );
	char ** papszFields = attr.Parse ( oVCTFeat.fields );
	int idx = 0;

	for ( auto iter = oVCTFeat.featuredefn->fielddefnlist.begin();
	        iter != oVCTFeat.featuredefn->fielddefnlist.end();
	        ++iter, ++idx )
	{
		switch ( CVCT2GDALFieldType ( iter->type ) )
		{
		case OFTInteger:
			oOGRFeat.SetField ( iter->name.c_str(), atoi ( papszFields[idx] ) );
			break;

		case OFTReal:
			oOGRFeat.SetField ( iter->name.c_str(), atof(  papszFields[idx] ) );
			break;

		case OFTString:
			oOGRFeat.SetField ( iter->name.c_str(), papszFields[idx] );
			break;

		default:
			oOGRFeat.SetField ( iter->name.c_str(), papszFields[idx] );
			break;
		}
	}

	return 0;
}

OGRGeometry * cvct2gdal::CVCT2GDALGeometry ( VCTGeometry & oVCTGeometry )
{
	OGRGeometry * poOGRGeometry = NULL;
	Geometry * poGeometry = NULL;

	if ( oVCTGeometry.geotype == GT_POINT )
	{
		PointParser oParser ( poVCTFile, poVCTDataSource );
		poGeometry = oParser.Parse ( oVCTGeometry );
		Geometry & cGeometry = *poGeometry;
		OGRGeometry * poPoint = new OGRPoint (cGeometry[0]->operator[](0).X,
		                                      cGeometry[0]->operator[](0).Y);
		poOGRGeometry = poPoint;
	}

	else if ( oVCTGeometry.geotype == GT_LINE )
	{
		LineParser  oParser ( poVCTFile, poVCTDataSource );
		poGeometry = oParser.Parse ( oVCTGeometry );
		Geometry & cGeometry = *poGeometry;
		OGRLineString * poLine = new OGRLineString();

		for ( auto iter = cGeometry[0]->begin();
		        iter != cGeometry[0]->end(); ++iter )
		{
			poLine->addPoint ( iter->X, iter->Y );
		}

		poOGRGeometry = poLine;
	}

	else if ( oVCTGeometry.geotype == GT_POLYGON )
	{
		PolyParser oParser ( poVCTFile, poVCTDataSource );
		poGeometry = oParser.Parse ( oVCTGeometry );
		Geometry & cGeometry = *poGeometry;
		int nParts = cGeometry.size();

		if (nParts == 1)
		{
			OGRPolygon * poOGRPoly = NULL;
			OGRLinearRing * poRing = NULL;
			poOGRGeometry = poOGRPoly = new OGRPolygon();
			poRing = CreateLinearRing(*cGeometry[0]);
			poOGRPoly->addRingDirectly(poRing);
		}

		else
		{
			OGRPolygon ** tabPolygons = new OGRPolygon*[nParts];

			for ( int iRing = 0; iRing != nParts; ++iRing )
			{
				tabPolygons[iRing] = new OGRPolygon();
				tabPolygons[iRing]->addRingDirectly ( CreateLinearRing ( *cGeometry[iRing] ) );
			}

			int isValidGeometry;
			const char * papszOptions[] = { "METHOD=ONLY_CCW", NULL };
			poOGRGeometry = OGRGeometryFactory::organizePolygons (
			                    ( OGRGeometry ** ) tabPolygons, nParts, &isValidGeometry, papszOptions );

			delete[] tabPolygons;
		}
	}

	DestroyGeometry(poGeometry);

	return poOGRGeometry;
}

void cvct2gdal::CVCT2GDALInit( VCTDataSource * poVCTDS )
{
	poVCTFile = poVCTDS->GetFile();
	poVCTDataSource = poVCTDS;
}

void cvct2gdal::CVCT2GDALLayer( VCTLayer * poVCTLayer, OGRLayer * poOGRLayer )
{
	OGRFeatureDefn * poOGRFeatDefn =
	    cvct2gdal::CVCT2GDALFeatureDefn ( & (poVCTLayer->featuredefn ) );

	for ( int ifield = 0; ifield < poOGRFeatDefn->GetFieldCount(); ++ifield )
	{
		poOGRLayer->CreateField ( poOGRFeatDefn->GetFieldDefn ( ifield ) );
	}

	for (auto ifeat = 0; ifeat != poVCTLayer->GetFeatureCount(); ++ifeat)
	{
		VCTFeature * poVCTFeat = poVCTLayer->GetFeatureByIndex(ifeat);
		//注意：feature在析构时，会将featuredefn也析构
		//将featuredefn复制后传入
		OGRFeature * poOGRFeat = cvct2gdal::CVCT2GDALFeature ( poVCTFeat, poOGRFeatDefn->Clone());
		poOGRLayer->CreateFeature ( poOGRFeat );

		//在window下，释放内存会报错
#ifndef WIN32
		OGRFeature::DestroyFeature(poOGRFeat);
#endif

	}

	//在window下，释放内存会报错
#ifndef WIN32
	poOGRFeatDefn->Release();
#endif
}
