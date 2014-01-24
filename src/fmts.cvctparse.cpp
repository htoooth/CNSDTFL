#include <vector>
#include <string>
#include <algorithm>

#include <cpl_string.h>

#include "fmts.cvctparse.h"
#include "port.debug.h"

#include "fmts.csr.h"

using namespace cvctparse;

size_t ParserIndex::Read ( char ** ppszdata, cvct::FilePos startpos, cvct::FilePos endpos )
{
	size_t ncount = static_cast<size_t> ( endpos - startpos ); //最后一位放NULL
	char * pTemp = ( char * ) CPLMalloc ( ncount + 1 );
	*ppszdata = pTemp;
	VSIFSeekL ( vctfile, startpos, SEEK_SET );
	pTemp[ncount] = NULL;
	return VSIFReadL ( pTemp, 1, ncount, vctfile );
}

GeometryPoint  parsepoint(char * pointdata)
{
	char ** poXY = CSLTokenizeString2 (pointdata, " \n\r\t,", 0 );
	GeometryPoint pPoint(atof ( poXY[0] ), atof ( poXY[1] ));
	CSLDestroy ( poXY );
	return pPoint ;
}

Geometry * PointParser::Parse ( cvct::VCTGeometry & geo )
{
	char * pTemp = NULL;
	Read ( &pTemp, geo.start_pos, geo.end_pos );
	GeometryPart * t = new GeometryPart();;
	t->push_back(parsepoint(pTemp));
	Geometry * gt = new Geometry();
	gt->push_back ( t );
	CPLFree ( pTemp );
	return gt;
}

Geometry * LineParser::Parse ( cvct::VCTGeometry & geo )
{
	char * pTemp = NULL;
	Read ( &pTemp, geo.start_pos, geo.end_pos );
	char ** poPoint = CSLTokenizeString2 ( pTemp, " \n\r\t", 0 );

	GeometryPart * t = new GeometryPart();

	for ( int i = 0; i != CSLCount ( poPoint ); ++i )
	{
		t->push_back(parsepoint(poPoint[i]));
	}

	Geometry * gt = new Geometry();
	gt->push_back ( t );
	CSLDestroy ( poPoint );

	CPLFree ( pTemp );

	return gt;
}

Geometry * PolyParser::Parse ( cvct::VCTGeometry & geo )
{
	typedef std::vector<std::string> Ring;//存线
	typedef std::vector<Ring> RingList;//存环

	Ring pt;
	RingList ringlist;

	char * pTemp = NULL;
	Read ( &pTemp, geo.start_pos, geo.end_pos );
	char ** poLine = CSLTokenizeString2 ( pTemp, " \n\r\t,", 0 );

	Geometry * pGeo = new Geometry();

	//由线转面
	//线变成一个环
	//变成一个环
	//先判断是否反转
	//查找对象
	//变成一个环
	poLine = CSLAddString(poLine, "0");

	for ( int i = 0; i != CSLCount ( poLine ); ++i )
	{
		if ( EQUAL ( poLine[i], "0" ) )
		{
			ringlist.push_back ( pt );
			pt.clear();
		}

		else
		{
			pt.push_back ( std::string ( poLine[i] ) );
		}
	}

	for ( auto iRing = ringlist.begin(); iRing != ringlist.end(); ++iRing )
	{
		GeometryPart * ring = new GeometryPart();

		for ( auto iLine = iRing->begin(); iLine != iRing->end(); ++iLine )
		{
			bool bset;//true，表示反方向，false,正方向
			int  objid = atoi(iLine->c_str());

			//查找是否有“-”号,是否反转
			if ( objid > 0 )
			{
				bset = false;
			}

			else
			{
				objid = -objid;
				bset = true;
			}

			//在所有的要素中，查找线，或面对像
			auto it = vctindex->GetFeatureById(objid);

			Geometry * geoline = NULL;

			//解析线对象
			if (it->geotype == cvct::GT_LINE)
			{
				LineParser iline(vctfile, vctindex);
				geoline = iline.Parse ( it->geometry );
			}

			//解析面对象
			else if (it->geotype == cvct::GT_POLYGON)
			{
				PolyParser ipoly(vctfile, vctindex);
				geoline = ipoly.Parse ( it->geometry );
			}

			//将线对象放到环中
			if ( bset )
			{
				ring->insert ( ring->end(), geoline->operator[](0)->rbegin(), geoline->operator[](0)->rend() );
			}

			else
			{
				ring->insert ( ring->end(), geoline->operator[](0)->begin(), geoline->operator[](0)->end() );
			}

		}

		if (ring->empty())
		{
			continue;
		}

		else
		{
			pGeo->push_back ( ring );
		}
	}

	CSLDestroy ( poLine );
	CPLFree ( pTemp );

	return pGeo;
}

///返回值自己解决内存释放
char ** AttrParser::Parse( cvct::VCTFieldValue & fieldvalue)
{
	char * popszInput = NULL;
	char * popszOutput = NULL;
	Read ( &popszInput, fieldvalue.start_pos, fieldvalue.end_pos );

    //最后多了两个空元素，因为行后面有回车和换行符，同时又允许空的元素存在
	char ** poFields = 
	    CSLTokenizeString2 ( popszInput, " \n\r\t,", CSLT_ALLOWEMPTYTOKENS );

	CPLFree(popszInput);
	return poFields;
}

void cvctparse::DestroyGeometry( Geometry * poGeometry )
{
	for (auto iter = poGeometry->begin(); iter != poGeometry->end(); ++iter)
	{
		delete *iter;
	}
}

OGRSpatialReference SRSParser::Parse(cvct::VCTSpatialReference & csrs)
{
    char * pszInput=NULL;
    Read(&pszInput,csrs.start_pos,csrs.end_pos);
    char ** popszItems=
        CSLTokenizeString2(pszInput,"\n\r\t",CSLT_ALLOWEMPTYTOKENS);
    
    BUGX(CSLPrint(popszItems,NULL));

    OGRSpatialReference ogrsrs=csr::CSRS2OGRSRS(csr::ExtractCSRFromAI(popszItems));

    CPLFree(pszInput);
    CSLDestroy(popszItems);

    return ogrsrs;
}