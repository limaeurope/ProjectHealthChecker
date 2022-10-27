#include	"ResultTable.hpp"
#include	"DGFileDialog.hpp"
#include	"../Data/SettingsSingleton.hpp"
#include	"LibXLExtended.hpp"
#include	"../Utils/Utils.hpp"
#define UNISTR_TO_LIBXLSTR(str) (str.ToUStr ())

// -----------------------------------------------------------------------------
// Open the selected XLSX file
// -----------------------------------------------------------------------------
bool	GetOpenFile(IO::Location* const dloc,
	const char* const fileExtensions,
	const GS::UniString& filterText,
	const DG::FileDialog::Type i_type = DG::FileDialog::Type::OpenFile)
{
	if (dloc == nullptr)
		return false;

	FTM::TypeID	retID;
	FTM::FileTypeManager	ftman("MyManager");
	FTM::FileType			type(nullptr, fileExtensions, 0, 0, 0);
	FTM::TypeID				id = FTM::FileTypeManager::SearchForType(type);
	if (id == FTM::UnknownType)
		id = ftman.AddType(type);

	DG::FileDialog	dlg(i_type);

	if (!dlg.Invoke())
		return false;

	*dloc = dlg.GetSelectedFile();
	return true;
}

void ResultSheet::AddItem(const GS::UniString& i_sItem,
	ResultRow& i_reportRow,
	const UInt16 i_pos)
	// Adds an sheetItem to both the UI report and the .xlsx output
{
	bool isEmpty = false;
	for (auto _i : i_reportRow.ToArray())
		isEmpty |= (bool)_i;

	if (!isEmpty && !SETTINGS().CheckBoxData[Zero_checkbox]) return;

	char sItem[256], _sNumberOfWalls[256], sInt[256];

	rowS.Add(i_sItem, i_reportRow);

	itoa(i_reportRow[0], sInt, 10);
	//FIXME
	sprintf(_sNumberOfWalls, "%s: %s", i_sItem.ToCStr().Get(), sInt);
	DGListInsertItem(32400, 2, DG_LIST_BOTTOM);
	DGListSetItemText(32400, 2, DG_LIST_BOTTOM, GS::UniString(_sNumberOfWalls));
}

void ResultSheet::AddItem(const GS::UniString& i_sItem)
{
	rowS.Add(i_sItem, ResultRow{});

	DGListInsertItem(32400, 2, DG_LIST_BOTTOM);
	DGListSetItemText(32400, 2, DG_LIST_BOTTOM, i_sItem);
}

void ResultTable::ExportReportToExcel()
{
	 BookExtended* book = (BookExtended*)xlCreateXMLBook();

	for (auto &sheetItem : sheetDict) {
		const GS::UniString _k = *sheetItem.key;
		SheetExtended* sheet = book->addSheet(UNISTR_TO_LIBXLSTR(_k));
		sheet->setCol(0, 4, COLUMN_WIDTH);

		auto _v = sheetItem.value;

		for (UIndex ii = 0; ii < _v->header.GetSize(); ++ii) {
			sheet->writeStr(0, ii, UNISTR_TO_LIBXLSTR(_v->header[ii]));
		}

		UIndex ii = 1;

		for (auto &rowItem : _v->rowS.Keys())
		{
			if	(sheetItem.value->rowS[rowItem].GetSize() 
				|| SETTINGS().CheckBoxData[Zero_checkbox] 
				|| _v->isZeroWrittenOut
				)
			{
				sheet->writeStr(ii, 0, rowItem.ToUStr());
				sheet->writeNum(ii++, 1, sheetItem.value->rowS[rowItem]);
			}
		}
	}

	GS::UniString filepath;

	if (!SETTINGS().CheckBoxData[AutoExport])
	{ 
		IO::Location xlsFileLoc;
		if (!GetOpenFile(&xlsFileLoc, "xlsx", "*.xlsx", DG::FileDialog::Save))
			return;

		xlsFileLoc.ToPath(&filepath);
	}
	else
	{
		filepath = SETTINGS().GetExport();
	}

	DBVERIFY(book->save(UNISTR_TO_LIBXLSTR(filepath)));
	book->release();
}

void ResultSheet::SetHeader(const GS::Array<int>& i_reportDataHeader)
{
	for (auto& _header : i_reportDataHeader)
		header.Push(GSFR((IntStr)_header));
}

