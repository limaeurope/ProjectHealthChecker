#include	"Table.hpp"
#include	"SettingsSingleton.hpp"

void SetHeader(const GS::UniString& i_sTable, const ReportDataHeader& i_reportDataHeader)
{
	if (!SETTINGS().ReportHeaderS.ContainsKey(i_sTable))
		SETTINGS().ReportHeaderS.Add(i_sTable, i_reportDataHeader);
	SETTINGS().ReportHeaderS[i_sTable] = i_reportDataHeader;
}

//void AddSum(const GS::UniString& i_sTable) {
//	//TODO
//}

void AddItem(const GS::UniString& i_sTable,
	const GS::UniString& i_sItem, 
	const UInt32 i_iItemNumber)
	// Adds an item to both the UI report and the .xlsx output
{
	if (!i_iItemNumber && !SETTINGS().CheckBoxData[ZERO_CHECKBOX]) return;

	char sInt[256];

	itoa(i_iItemNumber, sInt, 10);

	if (!SETTINGS().ReportData.ContainsKey(i_sTable))
	{
		ReportData _rd{};
		SETTINGS().ReportData.Add(i_sTable, _rd);
	}

	SETTINGS().ReportData[i_sTable].Add(i_sItem, i_iItemNumber);

	GS::UniString us = GS::UniString(i_sItem) + ": " + GS::UniString(sInt);
	auto _ul = us.GetLength();
	DGListInsertItem(32400, 2, DG_LIST_BOTTOM); 
	DGListSetItemText(32400, 2, DG_LIST_BOTTOM, us);
}

//void AddList(const GS::UniString& i_sTable, 
//	const GS::UniString& i_sItem, 
//	const UInt32 i_iItemNumber, 
//	CntlDlgData& i_cntlDlgData)
//{
//	if (!i_cntlDlgData.reportData.ContainsKey(i_sTable))
//	{
//		ReportData _rd{};
//		i_cntlDlgData.reportData.Add(i_sTable, _rd);
//	}
//
//	i_cntlDlgData.reportData[i_sTable].Add(i_sItem, i_iItemNumber);
//}