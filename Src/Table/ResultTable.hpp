#ifndef _RESULTTABLE_HPP
#define _RESULTTABLE_HPP

#include	"../APIEnvir.h"
#include	"ACAPinc.h"					// also includes APIdefs.h
#include	"../APICommon.h"
#include	"LibXL/libxl.h"
#include	"ResultRow.hpp"
#include	"Utils/Utils.hpp"


class ResultSheet
{
public:
	ResultSheet() {};
	ResultSheet(GS::UniString name): sName(name){};
	GS::UniString								sName;
	GS::Array<GS::UniString>					header;
	GS::HashTable<GS::UniString, ResultRow>		rowS;

	void AddItem(const GS::UniString& i_sItem,
		ResultRow& i_reportRow,
		const UInt16 i_pos = 0);
	inline void AddItem(const GS::UniString& i_sItem,
		const GS::Array<UInt32> i_iItemNumberS,
		const UInt16 i_pos = 0) { return AddItem(i_sItem, ResultRow{ i_iItemNumberS }, i_pos); };
	inline void AddItem(const GS::UniString& i_sItem,
		const UInt32 i_iItemNumber,
		const UInt16 i_pos = 0) { return AddItem(i_sItem, ResultRow{ i_iItemNumber }, i_pos); };
	inline void AddItem(const IntStr i_sItem,
		const UInt32 i_iItemNumber,
		const UInt16 i_pos = 0) { return AddItem(GSFR(i_sItem) , ResultRow{ i_iItemNumber }, i_pos); };
	inline void SetHeader(const GS::Array<GS::UniString>& i_reportDataHeader) { header = i_reportDataHeader; };
	void SetHeader(const GS::Array<IntStr>& i_reportDataHeader);
};

class ResultTable {
public:
	GS::HashTable<GS::UniString, ResultSheet>	sheetDict;
	void ExportReportToExcel();
};

#endif //_RESULTTABLE_HPP

