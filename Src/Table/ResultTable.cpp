#include	"ResultTable.hpp"
#include	"DGFileDialog.hpp"
#include	"../SettingsSingleton.hpp"
#include	"Excel.hpp"
#define UNISTR_TO_LIBXLSTR(str) (str.ToUStr ())


void ResultSheet::AddItem(const GS::UniString& i_sTable,
	const GS::UniString& i_sItem,
	const UInt32 i_iItemNumber)
	// Adds an item to both the UI report and the .xlsx output
{
	CntlDlgData cntlDlgData = SettingsSingleton::GetInstance()->cntlDlgData;

	if (!i_iItemNumber && !cntlDlgData.iAddZeroValues) return;

	char sItem[256], _sNumberOfWalls[256], sInt[256];

	itoa(i_iItemNumber, sInt, 10);

	if (!cntlDlgData.reportData.ContainsKey(i_sTable))
	{
		ReportData _rd{};
		cntlDlgData.reportData.Add(i_sTable, _rd);
	}

	cntlDlgData.reportData[i_sTable].Add(i_sItem, i_iItemNumber);

	sprintf(_sNumberOfWalls, "%s: %s", i_sItem.ToCStr().Get(), sInt);
	DGListInsertItem(32400, 2, DG_LIST_BOTTOM);
	DGListSetItemText(32400, 2, DG_LIST_BOTTOM, GS::UniString(_sNumberOfWalls));
}

void ResultTable::ExportReportToExcel()
{
	libxl::Book* book = xlCreateXMLBook();

	for (auto item : SettingsSingleton::GetInstance()->cntlDlgData.reportData) {
		const GS::UniString _k = *item.key;
		libxl::Sheet* sheet = book->addSheet(UNISTR_TO_LIBXLSTR(_k));
		sheet->setCol(0, 1, 50.0);

		GS::Array<GS::UniString> titles =
		{ "Object", "Number" };

		for (UIndex ii = 0; ii < titles.GetSize(); ++ii) {
			sheet->writeStr(0, ii, UNISTR_TO_LIBXLSTR(titles[ii]));
		}

		UIndex ii = 1;

		for (auto iitem : *item.value)
		{
			if (iitem.value > 0 || SettingsSingleton::GetInstance()->cntlDlgData.iAddZeroValues)
			{
				sheet->writeStr(ii, 0, iitem.key->ToUStr());
				sheet->writeNum(ii++, 1, *iitem.value);
			}
		}
	}

	IO::Location xlsFileLoc;
	if (!GetOpenFile(&xlsFileLoc, "xlsx", "*.xlsx", DG::FileDialog::Save))
		return;

	GS::UniString filepath;
	xlsFileLoc.ToPath(&filepath);

	DBVERIFY(book->save(UNISTR_TO_LIBXLSTR(filepath)));
	book->release();
}