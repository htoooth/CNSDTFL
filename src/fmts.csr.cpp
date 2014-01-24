#include "fmts.csr.h"

using namespace csr;

char ** ExtractCsrsString ( char ** additonalinformation )
{
	char ** csrsstring = NULL;

	static const char * csrshead[] =
	{
		"CoordinateSystemType",
		"XYUnit",
		"ZUnit",
		"Spheroid",
		"PrimeMeridian",
		"Projection",
		"Parameters",
		"VerticalDatum",
		"TemporalReferenceSystem"
	};
	int size = 9;

	for ( int i = 0; i < size; ++i )
	{
		int iindex = CSLFindName ( additonalinformation, csrshead[i] );

		if ( iindex != -1 )
		{
			csrsstring = CSLAddString ( csrsstring, additonalinformation[iindex] );
		}

		else
		{
			continue;
		}
	}

	return csrsstring;
}

csr::coordinate_system StructCSRS ( char ** csrsstring )
{
	coordinate_system cs;
	static const char * csrshead[] =
	{
		"CoordinateSystemType",
		"XYUnit",
		"ZUnit",
		"Spheroid",
		"PrimeMeridian",
		"Projection",
		"Parameters",
		"VerticalDatum",
		"TemporalReferenceSystem"
	};
	int size = 9;

	for ( int i = 0; i < size; ++i )
	{
		char ** poToken = CSLTokenizeString2 (
		                      CSLFetchNameValue ( csrsstring, csrshead[i] ),
		                      ",", CSLT_ALLOWEMPTYTOKENS );

		switch ( i )
		{
		case 0:
			if ( EQUAL ( poToken[0], "C" ) )
			{
				cs.CoordinateSystemType = COORDINATESYSTEM_C;
			}

			else if ( EQUAL ( poToken[0], "D" ) )
			{
				cs.CoordinateSystemType = COORDINATESYSTEM_D;
			}

			else if ( EQUAL ( poToken[0], "P" ) )
			{
				cs.CoordinateSystemType = COORDINATESYSTEM_P;
			}

			break;

		case 1:
			if ( EQUAL ( poToken[0], "M" ) )
			{
				cs.XYUnit.Type = UN_M;
				cs.XYUnit.Parameters.FullName = "Meter";
				cs.XYUnit.Parameters.UnitFactor = 1.0; //Default
				cs.XYUnit.Parameters.UnitType = UNIT_LENGTH;
			}

			else if ( EQUAL ( poToken[0], "D" ) )
			{
				cs.XYUnit.Type = UN_D;
				cs.XYUnit.Parameters.FullName = "Degree";
				cs.XYUnit.Parameters.UnitType = UNIT_ANGLE;
				cs.XYUnit.Parameters.UnitFactor = 1.0;
			}

			else
			{
				cs.XYUnit.Type = UN_O;
				cs.XYUnit.Parameters.FullName = poToken[1];
				cs.XYUnit.Parameters.UnitFactor = CPLAtof ( poToken[3] );
				cs.XYUnit.Parameters.UnitType = UNIT_OTHER;
			}

			break;

		case 2:
			if ( EQUAL ( poToken[0], "M" ) )
			{
				cs.ZUnit.Type = UN_M;
				cs.ZUnit.Parameters.FullName = "Meter";
				cs.ZUnit.Parameters.UnitFactor = 1.0; //Default
				cs.ZUnit.Parameters.UnitType = UNIT_LENGTH;
			}

			else
			{
				cs.XYUnit.Type = UN_O;
				cs.XYUnit.Parameters.FullName = poToken[0];
				cs.XYUnit.Parameters.UnitFactor = CPLAtof ( poToken[2] );
				cs.XYUnit.Parameters.UnitType = UNIT_OTHER;
			}

			break;

		case 3:
			cs.Spheroid.Type = poToken[0];
			cs.Spheroid.Semi_MajorAxis = CPLAtof ( poToken[1] );
			cs.Spheroid.ReciprocalOfFlattening = CPLAtof ( poToken[2] );
			break;

		case 4:
			if ( EQUAL ( poToken[0], "Greenwich" ) )
			{
				cs.PrimeMeridian.Type = PM_GREENWICH;
				cs.PrimeMeridian.Parameters.FullName = "Greenwich";
				cs.PrimeMeridian.Parameters.GreenwichLongitude = 0.0;
			}

			else
			{
				cs.PrimeMeridian.Type = PM_OTHER;
				cs.PrimeMeridian.Parameters.FullName = poToken[0];
				cs.PrimeMeridian.Parameters.GreenwichLongitude = CPLAtof ( poToken[1] );
			}

			break;

		case 5:
			cs.Projection.FullName = poToken[0];
			break;

		case 6:
			cs.Projection.Parameters.OiginLatitude = CPLAtof ( poToken[0] );
			cs.Projection.Parameters.OiginLongitude = CPLAtof ( poToken[1] );
			cs.Projection.Parameters.StandardParallelOne = CPLAtof ( poToken[2] );
			cs.Projection.Parameters.StandardParallelTwo = CPLAtof ( poToken[3] );
			cs.Projection.Parameters.Azimuth = CPLAtof ( poToken[4] );
			cs.Projection.Parameters.ScaleFactor = CPLAtof ( poToken[5] );
			cs.Projection.Parameters.FalseEasting = CPLAtof ( poToken[6] );
			cs.Projection.Parameters.FalseNorthing = CPLAtof ( poToken[7] );

			if ( EQUAL ( poToken[8], "3" ) )
			{
				cs.Projection.Parameters.ZoneSize.Type = ZOME_SIZE_3;
				cs.Projection.Parameters.ZoneSize.Size = 3.0;
			}

			else if ( EQUAL ( poToken[8], "6" ) )
			{
				cs.Projection.Parameters.ZoneSize.Type = ZOME_SIZE_6;
				cs.Projection.Parameters.ZoneSize.Size = 6.0;
			}

			else if ( EQUAL ( poToken[8], "1.5" ) )
			{
				cs.Projection.Parameters.ZoneSize.Type = ZOME_SIZE_1;
				cs.Projection.Parameters.ZoneSize.Size = 1.5;
			}

			else
			{
				cs.Projection.Parameters.ZoneSize.Type = ZOME_SIZE_OTHER;
				cs.Projection.Parameters.ZoneSize.Size = CPLAtof ( poToken[8] );
			}

			cs.Projection.Parameters.ZoneNum = atoi ( poToken[9] ); //???doule-->int
			break;

		case 7:
			if ( EQUAL ( poToken[0], "1985国家高程基准" ) )
			{
				cs.Datum.Type = DATUM_HUANGHAI1985;
				cs.Datum.Parameters.FullName = poToken[0];
				cs.Datum.Parameters.Dvalue = 0.0;
			}

			else if ( EQUAL ( poToken[0], "1956黄海高程系统" ) )
			{
				cs.Datum.Type = DATUM_HUANGHAI1956;
				cs.Datum.Parameters.FullName = poToken[0];
				cs.Datum.Parameters.Dvalue = 0.0; //??????
			}

			else
			{
				cs.Datum.Type = DATUM_OTHER;
				cs.Datum.Parameters.FullName = poToken[0];
				cs.Datum.Parameters.Dvalue = CPLAtof ( poToken[1] ); //??????
			}

			break;

		case 8:
			if ( EQUAL ( poToken[0], "协调时间" ) )
			{
				cs.TemporalReferenceSystem.Type = TEMPORAL_UTC;
				cs.TemporalReferenceSystem.Parameters.FullName = "UTC";
				cs.TemporalReferenceSystem.Parameters.Dvalue = "+0000";
			}

			else
			{
				cs.TemporalReferenceSystem.Type = TEMPORAL_OTHER;
				cs.TemporalReferenceSystem.Parameters.FullName = poToken[0];
				cs.TemporalReferenceSystem.Parameters.Dvalue = poToken[1];
			}

			break;
		}
	}

	return cs;
}

csr::coordinate_system csr::ExtractCSRFromAI ( char ** additonalinformation )
{
	char ** csrsstring = ExtractCsrsString ( additonalinformation );
	csr::coordinate_system cs = StructCSRS ( csrsstring );
	CSLDestroy ( csrsstring );
	return cs;
}

OGRSpatialReference csr::CSRS2OGRSRS ( csr::coordinate_system cs )
{
	OGRSpatialReference ogr;

	if ( EQUAL ( cs.Spheroid.Type.c_str(), "克拉索夫斯基椭球（1940）" ) )
	{
		ogr.SetWellKnownGeogCS ( "EPSG:4214" );
	}

	else if ( EQUAL ( cs.Spheroid.Type.c_str(), "IAG-75" ) )
	{
		ogr.SetWellKnownGeogCS ( "EPSG:4610" );
	}

	else if ( EQUAL ( cs.Spheroid.Type.c_str(), "WGS-84" ) )
	{
		ogr.SetWellKnownGeogCS ( "WGS84" );
	}

	if ( cs.XYUnit.Type == UN_M )   //如果是千米？
	{
		ogr.SetLinearUnits ( "Meter", cs.XYUnit.Parameters.UnitFactor );
	}

	if ( cs.XYUnit.Type == UN_D )
	{
		ogr.SetAngularUnits ( "Degree", cs.XYUnit.Parameters.UnitFactor );
	}

	return ogr;
}

void csr::CSR2WKT ( coordinate_system csr, char ** papszWKT )
{
	OGRSpatialReference ogr = CSRS2OGRSRS ( csr );
	ogr.exportToWkt ( papszWKT );
}
