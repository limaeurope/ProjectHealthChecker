
#ifndef _DATASTRUCTS_HPP
#define _DATASTRUCTS_HPP

#include	"APIEnvir.h"
#include	"ACAPinc.h"					// also includes APIdefs.h
#include	"APICommon.h"

typedef GS::HashTable<GS::UniString, UInt32> ReportData;

struct CntlDlgData {
	Int32 iAddZeroValues;
	GS::HashTable<GS::UniString, ReportData> reportData;
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

#endif	//_DATASTRUCTS_HPP