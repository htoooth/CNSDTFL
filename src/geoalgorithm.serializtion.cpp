#include "geoalgorithm.serializtion.h"
#include "port.designpattern.h"

#include <string>

class FeatToken
{
	friend class port::Singleton<FeatToken>;
public:
	const char * pszFieldToken;
	const char * pszFieldDefnToken;
	const char * pszFieldDefnItemToken;
	const char * pszFeatToken;
	const char * pszFeatDefnToken;

	char * pszFeat;
	char * pszGeometry;
	char * pszField;
	char * pszFieldDefn;
	char * pszFeatDefn;

	const char * pszFeatFormat;
	const char * pszFeatDefnFormat;
	const char * pszFieldDefnFormat;

private:
	const std::string featuredefn;
	const std::string geometry;
	const std::string field;
	const std::string featsq;
	const std::string context1;
	const std::string context2;

	const std::string name;
	const std::string type;
	const std::string fielddefn;
	const std::string width;
	const std::string precision;
	const std::string featdefnsq;
	const std::string fielddefnsq;
	const std::string fielddefnitemsq;

	std::string featformat;
	std::string featdefnformat;
	std::string fielddefnformat;

	FeatToken():
		featuredefn("featuredefn"),
		geometry("geometry"),
		field("field"),
		featsq(";"),
		context1(":%s"),
		context2(":%d"),
		name("name"),
		type("type"),
		fielddefn("fielddefn"),
		width("width"),
		precision("precision"),
		featdefnsq(","),
		fielddefnsq("."),
		fielddefnitemsq("]"),
		pszFeat(NULL),
		pszGeometry(NULL),
		pszField(NULL),
		pszFieldDefn(NULL),
		pszFeatDefn(NULL)
	{
		//featuredefn:%s;geometry:%s;field:%s
		featformat = featuredefn + context1 + featsq
		             + geometry + context1 + featsq
		             + field + context1;
		//name:%s,type:%d,fielddefn:%s
		featdefnformat = name + context1 + featdefnsq
		                 + type + context2 + featdefnsq
		                 + fielddefn + context1;
		//name:%s.type:%d.width:%d.precision:%d
		fielddefnformat = name + context1 + fielddefnsq
		                  + type + context2 + fielddefnsq
		                  + width + context2 + fielddefnsq
		                  + precision + context2;

		pszFeatFormat = featformat.c_str();
		pszFeatDefnFormat = featdefnformat.c_str();
		pszFieldDefnFormat = fielddefnformat.c_str();

		pszFieldToken = featdefnsq.c_str();
		pszFieldDefnToken = fielddefnsq.c_str();
		pszFieldDefnItemToken = fielddefnitemsq.c_str();
		pszFeatToken = featsq.c_str();
		pszFeatDefnToken = featdefnsq.c_str();
	};

	FeatToken(const FeatToken &) {};
	FeatToken & operator=(const FeatToken &) {};
};

const char * OGRFeat_PACK( OGRFeature * oFeat )
{
	FeatToken * poFeatToken = port::Singleton<FeatToken>::CreateInstance();

	//将数据结构定义成
	//key:value形式
	if (poFeatToken->pszFeat != NULL)
	{
		CPLFree(poFeatToken->pszFeat);
		poFeatToken->pszFeat = NULL;
	}

	const char * pszFormat = poFeatToken->pszFeatFormat;
	char * pszFeatDefn = CPLStrdup(OGRFeatDefn_PACK(oFeat->GetDefnRef()));
	char * pszGeometry = CPLStrdup(OGRGeometry_PACK(oFeat->GetGeometryRef()));
	char * pszFields = CPLStrdup(OGRField_PACK(oFeat));
	int nFormat = strlen(pszFormat);
	int nFeatDefn = strlen(pszFeatDefn);
	int nGeometry = strlen(pszGeometry);
	int nFields = strlen(pszFields);

	poFeatToken->pszFeat = (char *)CPLMalloc(nFormat + nGeometry + nFeatDefn + nFields + 1);
	sprintf(poFeatToken->pszFeat, pszFormat, pszFeatDefn, pszGeometry, pszFields);

	CPLFree(pszFeatDefn);
	CPLFree(pszGeometry);
	CPLFree(pszFields);

	return poFeatToken->pszFeat;
}

const char * OGRFeatDefn_PACK( OGRFeatureDefn * oFeatDefn )
{
	FeatToken * poFeatToken = port::Singleton<FeatToken>::CreateInstance();

	if (poFeatToken->pszFeatDefn != NULL)
	{
		CPLFree(poFeatToken->pszFeatDefn);
		poFeatToken->pszFeatDefn = NULL;
	}

	const char * pszFormat = poFeatToken->pszFeatDefn;
	std::string sFieldDefn;
	std::string sq(poFeatToken->pszFieldDefnItemToken);

	for (int ifield = 0; ifield < oFeatDefn->GetFieldCount(); ++ifield)
	{
		const char * cTemp = OGRFieldDefn_PACK(oFeatDefn->GetFieldDefn(ifield));
		std::string iFieldDefn(cTemp);
		iFieldDefn += sq;
		sFieldDefn += iFieldDefn;
	}

	int nFormat = strlen(pszFormat + 10);
	int nFeatDefn = strlen(oFeatDefn->GetName());
	int nFieldDefn = strlen(sFieldDefn.c_str());

	poFeatToken->pszFeatDefn = (char *)CPLMalloc(nFormat + nFeatDefn + nFieldDefn + 1);
	sprintf(poFeatToken->pszFeatDefn, pszFormat,
	        oFeatDefn->GetName(), oFeatDefn->GetGeomType(), sFieldDefn.c_str());
	return poFeatToken->pszFeatDefn;
}

const char * OGRFieldDefn_PACK( OGRFieldDefn * oFielddefn )
{
	FeatToken * poFeatToken = port::Singleton<FeatToken>::CreateInstance();

	if (poFeatToken->pszFeatDefn != NULL)
	{
		CPLFree(poFeatToken->pszFeatDefn);
		poFeatToken->pszFeatDefn = NULL;
	}

	const char * pszFormat = poFeatToken->pszFieldDefnFormat;

	int nFormat = strlen(pszFormat + 10);
	int nFieldDefn = strlen(oFielddefn->GetNameRef());

	poFeatToken->pszFeatDefn = (char *)CPLMalloc(nFormat + nFieldDefn + 1);
	sprintf(poFeatToken->pszFeatDefn, pszFormat,
	        oFielddefn->GetNameRef(), oFielddefn->GetType(),
	        oFielddefn->GetWidth(), oFielddefn->GetPrecision());

	return poFeatToken->pszFeatDefn;
}

const char * OGRGeometry_PACK( OGRGeometry * oGeometry )
{
	FeatToken * poFeatToken = port::Singleton<FeatToken>::CreateInstance();

	if (poFeatToken->pszGeometry != NULL)
	{
		CPLFree(poFeatToken->pszGeometry);
		poFeatToken->pszGeometry = NULL;
	}

	oGeometry->exportToWkt(&poFeatToken->pszGeometry);
	return poFeatToken->pszGeometry;
}

const char * OGRField_PACK( OGRFeature * oFeat )
{
	FeatToken * poFeatToken = port::Singleton<FeatToken>::CreateInstance();

	if (poFeatToken->pszField != NULL)
	{
		CPLFree(poFeatToken->pszField);
		poFeatToken->pszField = NULL;
	}

	std::string sField;
	std::string Seq(poFeatToken->pszFieldToken);

	for (int ifeat = 0; ifeat < oFeat->GetFieldCount(); ++ifeat)
	{
		std::string sTemp(oFeat->GetFieldAsString(ifeat));
		sTemp += Seq;
		sField += sTemp;
	}

	poFeatToken->pszField = CPLStrdup(sField.c_str());
	return poFeatToken->pszField;
}

OGRFeature * OGRFeat_UNPACK( const char * pszFeat )
{
	FeatToken * poFeatToken = port::Singleton<FeatToken>::CreateInstance();
	char ** papszFeat = CSLTokenizeString2(pszFeat, poFeatToken->pszFeatToken, CSLT_ALLOWEMPTYTOKENS);
	const char * pszFeatDefn = CSLFetchNameValue(papszFeat, "featuredefn");
	const char * pszFieldValue = CSLFetchNameValue(papszFeat, "field");
	const char * pszGeometry = CSLFetchNameValue(papszFeat, "geometry");

	OGRFeatureDefn * poFeatDefn = OGRFeatDefn_UNPACK(pszFeatDefn);
	OGRGeometry * poGeometry = OGRGeometry_UNPACK(pszGeometry);
	OGRFeature * poFeat = new OGRFeature(poFeatDefn);
	poFeat->SetGeometryDirectly(poGeometry);
	poFeat = OGRField_UNPACK(poFeat, pszFieldValue);

	CSLDestroy(papszFeat);

	return poFeat;
}

OGRFeatureDefn * OGRFeatDefn_UNPACK( const char * pszFeatDefn )
{
	FeatToken * poFeatToken = port::Singleton<FeatToken>::CreateInstance();
	char ** papszFeatDefn = CSLTokenizeString2(pszFeatDefn, poFeatToken->pszFeatDefnToken, CSLT_ALLOWEMPTYTOKENS);
	const char * pszName = CSLFetchNameValue(papszFeatDefn, "name");
	const char * pszType = CSLFetchNameValue(papszFeatDefn, "type");
	const char * pszFields = CSLFetchNameValue(papszFeatDefn, "fielddefn");

	OGRFeatureDefn * poFeatDefn = new OGRFeatureDefn(pszName);
	poFeatDefn->SetGeomType((OGRwkbGeometryType)atoi(pszType));

	char ** papszFieldDefn = CSLTokenizeString2(pszFields, poFeatToken->pszFieldDefnItemToken, 0);

	for (int iField = 0; iField < CSLCount(papszFieldDefn); ++iField)
	{
		OGRFieldDefn * poOGRFieldDefn = OGRFieldDefn_UNPACK(papszFieldDefn[iField]);
		poFeatDefn->AddFieldDefn(poOGRFieldDefn);
		delete poOGRFieldDefn;
	}

	CSLDestroy(papszFeatDefn);
	CSLDestroy(papszFieldDefn);

	return poFeatDefn;
}

OGRFieldDefn * OGRFieldDefn_UNPACK( const char * pszFieldDefn )
{
	FeatToken * poFeatToken = port::Singleton<FeatToken>::CreateInstance();
	char ** papszFieldDefn = CSLTokenizeString2(pszFieldDefn, poFeatToken->pszFeatDefnToken, CSLT_ALLOWEMPTYTOKENS);

	const char * pszName = CSLFetchNameValue(papszFieldDefn, "name");
	const char * pszTpye = CSLFetchNameValue(papszFieldDefn, "type");
	const char * pszWidth = CSLFetchNameValue(papszFieldDefn, "width");
	const char * pszPreci = CSLFetchNameValue(papszFieldDefn, "precision");

	OGRFieldDefn * poFieldDefn = new OGRFieldDefn("", OFTInteger);
	poFieldDefn->SetName(pszName);
	poFieldDefn->SetType((OGRFieldType)atoi(pszTpye));
	poFieldDefn->SetWidth(atoi(pszWidth));
	poFieldDefn->SetPrecision(atoi(pszPreci));

	CSLDestroy(papszFieldDefn);
	return poFieldDefn;
}


OGRGeometry * OGRGeometry_UNPACK( const char * pszGeo )
{
	OGRGeometry * poGeo = NULL;
	char * pszNewGeo = CPLStrdup(pszGeo);
	OGRGeometryFactory::createFromWkt(&pszNewGeo, NULL, &poGeo);
	CPLFree(pszNewGeo);
	return poGeo;
}

OGRFeature * OGRField_UNPACK( OGRFeature * poFeature, const char * pszField )
{
	FeatToken * poFeatToken = port::Singleton<FeatToken>::CreateInstance();
	char ** papszField = CSLTokenizeString2(pszField, poFeatToken->pszField, CSLT_ALLOWEMPTYTOKENS);
	OGRFeatureDefn * poDefn = poFeature->GetDefnRef();

	for (int iField = 0; iField < poDefn->GetFieldCount(); ++iField)
	{
		switch (poDefn->GetFieldDefn(iField)->GetType())
		{
		case OFTString:
			poFeature->SetField(iField, papszField[iField]);
			break;

		case OFTInteger:
			poFeature->SetField(iField, atoi(papszField[iField]));
			break;

		case OFTReal:
			poFeature->SetField(iField, atof(papszField[iField]));
			break;

		case OFTDate:
		{
			OGRField sFld;
			const char * pszDateValue = papszField[iField];

			memset( &sFld, 0, sizeof(sFld) );

			if ( pszDateValue[2] == '/' && pszDateValue[5] == '/'
			        && strlen(pszDateValue) >= 10 )
			{
				sFld.Date.Month = (GByte)atoi(pszDateValue + 0);
				sFld.Date.Day   = (GByte)atoi(pszDateValue + 3);
				sFld.Date.Year  = (GInt16)atoi(pszDateValue + 6);
			}

			else
			{
				int nFullDate = atoi(pszDateValue);
				sFld.Date.Year = (GInt16)(nFullDate / 10000);
				sFld.Date.Month = (GByte)((nFullDate / 100) % 100);
				sFld.Date.Day = (GByte)(nFullDate % 100);
			}

			poFeature->SetField( iField, &sFld );
		}
		break;
		}
	}

	CSLDestroy(papszField);

	return poFeature;
}
