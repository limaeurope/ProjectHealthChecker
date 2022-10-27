#ifndef _DATASTRUCTS_HPP
#define _DATASTRUCTS_HPP

#include	"APIEnvir.h"
#include	"ACAPinc.h"					// also includes APIdefs.h
#include	"APICommon.h"
#include	"AttributeUsage.hpp"
#include	"ResultRow.hpp"


//typedef GS::HashTable<GS::UniString, ResultRow> ReportSheet;
typedef GS::Array<GS::UniString> ReportSheetHeader;


struct CntlDlgData {
	GS::HashSet<GS::UniString> filterStrings;
};


//----------------------------------------------------------------


struct AbstractData {};


struct AttributeData :AbstractData {
	AttributeUsage	attributeUsage;
	AttributeData(AttributeUsage au) :attributeUsage(au) {}
};


struct StringArrayData :AbstractData {
	GS::Array<GS::UniString> stringArray;
	StringArrayData(GS::Array<GS::UniString> sA) :stringArray(sA) {}
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
	API_AttributeIndex index;
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


struct AttributeReportObject: AbstractData
{
	GS::UniString name;
	short id;
	ResultRow resultRow;
};


template<typename T>
struct StructDefObject: AbstractData
{
	GS::HashTable<T, ResultRow>  table;
	StructDefObject(GS::HashTable<T, ResultRow> t) : table(t) {};
};


#endif	//_DATASTRUCTS_HPP

