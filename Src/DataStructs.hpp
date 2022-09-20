
#ifndef _DATASTRUCTS_HPP
#define _DATASTRUCTS_HPP

#include	"APIEnvir.h"
#include	"ACAPinc.h"					// also includes APIdefs.h
#include	"APICommon.h"

typedef GS::HashTable<GS::UniString, UInt32> ReportData;
//typedef GS::HashTable<GS::UniString, GS::Array<UInt32>> ReportMultiData;
typedef GS::Array<GS::UniString> ReportDataHeader;


struct CntlDlgData {
	Int32 iAddZeroValues;
	GS::Array<Int32>	CheckBoxData{};
	GS::HashTable<GS::UniString, ReportData> reportData;
	GS::HashTable<GS::UniString, ReportDataHeader> reportHeaders;
	GS::HashSet<GS::UniString> filterStrings;
};


struct AbstractData {};


struct StringData :AbstractData {
	GS::UniString string;
	StringData(GS::UniString s) :string(s) {}
};

//struct DataObject {};


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

#endif	//_DATASTRUCTS_HPP