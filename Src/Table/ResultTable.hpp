#ifndef _RESULTTABLE_HPP
#define _RESULTTABLE_HPP

#include	"../APIEnvir.h"
#include	"ACAPinc.h"					// also includes APIdefs.h
#include	"../APICommon.h"
#include	"LibXL/libxl.h"
#include	"ReportRow.hpp"

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
	
	void AddItem(const GS::UniString& i_sTable,
		const GS::UniString& i_sItem,
		const GS::Array<UInt32> i_iItemNumber,
		const UInt16 i_pos = 0);
	void ResultSheet::AddItem(const GS::UniString& i_sTable,
		const GS::UniString& i_sItem,
		const UInt32 i_iItemNumber,
		const UInt16 i_pos = 0);
	void ResultSheet::AddItem(const GS::UniString& i_sTable,
		const GS::UniString& i_sItem,
		ReportRow i_reportRow,
		const UInt16 i_pos);
};

class ResultTable {
public:
	GS::HashTable<GS::UniString, ResultSheet>	sheetDict;
	void ExportReportToExcel();
};

#endif //_RESULTTABLE_HPP