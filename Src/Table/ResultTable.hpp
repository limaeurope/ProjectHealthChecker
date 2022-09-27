#ifndef _RESULTTABLE_HPP
#define _RESULTTABLE_HPP

#include	"../APIEnvir.h"
#include	"ACAPinc.h"					// also includes APIdefs.h
#include	"../APICommon.h"
#include	"LibXL/libxl.h"

struct Row {
	GS::UniString	textField;
	GS::Array<Int32>	numberS;
};

class ResultSheet
{
public:
	ResultSheet() {};
	ResultSheet(GS::UniString name) :sName(name){};
	GS::Array<GS::UniString>	header;
	GS::Array<Row>				rowS;
	GS::UniString				sName;
	//GS::Array<Int32>			sumS;
	//bool						isSumAtTheTop;

	//void AddHeader(GS::Array<GS::UniString> i_headerS);
	//void AddSum(const GS::UniString& i_sTable);
	void AddItem(const GS::UniString& i_sTable,
		const GS::UniString& i_sItem,
		const UInt32 i_iItemNumber);
};

class ResultTable {
public:
	GS::HashTable<GS::UniString, ResultSheet>	sheetDict;
	//void AddSheet(GS::UniString name, ResultSheet sheet) { sheetDict.Add(name, sheet); }
	void ExportReportToExcel();
};

#endif //_RESULTTABLE_HPP