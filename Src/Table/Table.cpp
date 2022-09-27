#include	"Table.hpp"
#include	"../SettingsSingleton.hpp"

//void SetHeader(const GS::UniString& i_sTable, const ReportDataHeader& reportDataHeader, CntlDlgData& io_cntlDlgData)
//{
//	if (io_cntlDlgData.reportHeaders.ContainsKey(i_sTable)
//		for()
//}

//void AddSum(const GS::UniString& i_sTable) {
//	//TODO
//}

void AddItem(const GS::UniString& i_sTable,
	const GS::UniString& i_sItem, 
	const UInt32 i_iItemNumber)
	// Adds an item to both the UI report and the .xlsx output
{
	if (!i_iItemNumber && !SettingsSingleton::GetInstance().CheckBoxData[ZERO_CHECKBOX]) return;

	char sItem[256], _sNumberOfWalls[256], sInt[256];

	itoa(i_iItemNumber, sInt, 10);

	if (!SettingsSingleton::GetInstance().ReportData.ContainsKey(i_sTable))
	{
		ReportData _rd{};
		SettingsSingleton::GetInstance().ReportData.Add(i_sTable, _rd);
	}

	SettingsSingleton::GetInstance().ReportData[i_sTable].Add(i_sItem, i_iItemNumber);

	sprintf(_sNumberOfWalls, "%s: %s", i_sItem.ToCStr().Get(), sInt);
	DGListInsertItem(32400, 2, DG_LIST_BOTTOM);
	DGListSetItemText(32400, 2, DG_LIST_BOTTOM, GS::UniString(_sNumberOfWalls));
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