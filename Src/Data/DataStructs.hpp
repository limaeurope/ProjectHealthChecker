
#ifndef _DATASTRUCTS_HPP
#define _DATASTRUCTS_HPP

#include	"APIEnvir.h"
#include	"ACAPinc.h"					// also includes APIdefs.h
#include	"APICommon.h"
#include	"AttributeUsage.hpp"

typedef GS::HashTable<GS::UniString, UInt32> ReportData;
typedef GS::Array<GS::UniString> ReportDataHeader;


struct CntlDlgData {
	Int32 iAddZeroValues;
	GS::Array<Int32>	CheckBoxData{};
	GS::HashTable<GS::UniString, ReportData> ReportData;
	GS::HashTable<GS::UniString, ReportDataHeader> reportHeaders;
	GS::HashSet<GS::UniString> filterStrings;
};


struct AbstractData {};


struct AttributeData :AbstractData {
	AttributeUsage	attributeUsage;
	AttributeData(AttributeUsage au) :attributeUsage(au) {}
};


struct StringData :AbstractData {
	GS::UniString string;
	StringData(GS::UniString s) :string(s) {}
};


struct FileSizeReportObject : AbstractData
{
	GS::UniString path;
	GS::UniString name;
	UInt64 size;
	bool isEmbedded;
	API_LibraryTypeID libType;
	UInt32 nInstances = 0;
};


struct PolygonReportObject: AbstractData
{
	GS::UniString name;
	GS::HashTable<API_Guid, UInt32> arcsByHatch;
	UInt32 nArcs = 0;
};


struct LayerReportObject: AbstractData
{
	GS::UniString name;
	int nInstances;
};


#endif	//_DATASTRUCTS_HPP