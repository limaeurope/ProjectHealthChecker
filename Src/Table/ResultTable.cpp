#include	"ResultTable.hpp"
#include	"DGFileDialog.hpp"
#include	"../Data/SettingsSingleton.hpp"
#include	"Excel.hpp"
#define UNISTR_TO_LIBXLSTR(str) (str.ToUStr ())


void ResultSheet::AddItem(const GS::UniString& i_sTable,
	const GS::UniString& i_sItem,
	const GS::Array<UInt32> i_iItemNumberS,
	const UInt16 i_pos)
	// Adds an item to both the UI report and the .xlsx output
{
	bool isEmpty = false;
	for (auto _i : i_iItemNumberS)
		isEmpty |= (bool)_i;

	if (!isEmpty && !SETTINGS().CheckBoxData[ZERO_CHECKBOX]) return;

	char sItem[256], _sNumberOfWalls[256], sInt[256];

	if (!SETTINGS().ReportData.ContainsKey(i_sTable))
	{
		ReportData _rd{};
		SETTINGS().ReportData.Add(i_sTable, _rd);
	}

	SETTINGS().ReportData[i_sTable].Add(i_sItem, ReportRow{i_iItemNumberS});

	itoa(i_iItemNumberS[0], sInt, 10);

	sprintf(_sNumberOfWalls, "%s: %s", i_sItem.ToCStr().Get(), sInt);
	DGListInsertItem(32400, 2, DG_LIST_BOTTOM);
	DGListSetItemText(32400, 2, DG_LIST_BOTTOM, GS::UniString(_sNumberOfWalls));
}

void ResultSheet::AddItem(const GS::UniString& i_sTable,
	const GS::UniString& i_sItem,
	ReportRow i_reportRow,
	const UInt16 i_pos)
	// Adds an item to both the UI report and the .xlsx output
{
	bool isEmpty = false;
	for (auto _i : i_reportRow.ToArray())
		isEmpty |= (bool)_i;

	if (!isEmpty && !SETTINGS().CheckBoxData[ZERO_CHECKBOX]) return;

	char sItem[256], _sNumberOfWalls[256], sInt[256];

	if (!SETTINGS().ReportData.ContainsKey(i_sTable))
	{
		ReportData _rd{};
		SETTINGS().ReportData.Add(i_sTable, _rd);
	}

	SETTINGS().ReportData[i_sTable].Add(i_sItem, i_reportRow);

	itoa(i_reportRow[2], sInt, 10);

	sprintf(_sNumberOfWalls, "%s: %s", i_sItem.ToCStr().Get(), sInt);
	DGListInsertItem(32400, 2, DG_LIST_BOTTOM);
	DGListSetItemText(32400, 2, DG_LIST_BOTTOM, GS::UniString(_sNumberOfWalls));
}

void ResultSheet::AddItem(const GS::UniString& i_sTable,
	const GS::UniString& i_sItem,
	const UInt32 i_iItemNumber,
	const UInt16 i_pos)
{
	return AddItem(i_sTable, i_sItem, GS::Array<UInt32>{i_iItemNumber}, i_pos);
}

void ResultTable::ExportReportToExcel()
{
	libxl::Book* book = xlCreateXMLBook();

	for (auto item : SETTINGS().ReportData) {
		const GS::UniString _k = *item.key;
		libxl::Sheet* sheet = book->addSheet(UNISTR_TO_LIBXLSTR(_k));
		sheet->setCol(0, 3, COLUMN_WIDTH);

		GS::Array<GS::UniString> titles =
		{ "Object", "Number" };

		for (UIndex ii = 0; ii < titles.GetSize(); ++ii) {
			sheet->writeStr(0, ii, UNISTR_TO_LIBXLSTR(titles[ii]));
		}

		UIndex ii = 1;

		for (auto iitem : *item.value)
		{
			if (iitem.value > 0 || SETTINGS().CheckBoxData[ZERO_CHECKBOX])
			{
				sheet->writeStr(ii, 0, iitem.key->ToUStr());
				sheet->writeNum(ii++, 1, iitem.value->Get());
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