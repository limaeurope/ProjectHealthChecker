#include	"Excel.hpp"
#include	"LibXL/libxl.h"
#include	"SettingsSingleton.hpp"
#define UNISTR_TO_LIBXLSTR(str) (str.ToUStr ())

// -----------------------------------------------------------------------------
// Open the selected XLSX file into a library part
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


void	Do_ImportNamesFromExcel()
{
	IO::Location xlsFileLoc;
	if (!GetOpenFile(&xlsFileLoc, "xlsx", "*.xlsx", DG::FileDialog::OpenFile))
		return;

	GS::UniString filepath;
	xlsFileLoc.ToPath(&filepath);

	libxl::Book* book;
	bool isBookLoaded = false;

	book = xlCreateBook();
	if (book->load(filepath.ToUStr()))
		isBookLoaded = true;
	else
	{
		book = xlCreateXMLBook();
		if (book->load(filepath.ToUStr()))
			isBookLoaded = true;
	}

	if (isBookLoaded)
	{
		if (libxl::Sheet* sheet = book->getSheet(0))
		{
			for (int row = sheet->firstRow(); row < sheet->lastRow(); ++row)
			{
				if (const wchar_t* sFilter = sheet->readStr(row, 0))
					SETTINGS().FilterStrings.Add(GS::UniString(sFilter));
			}
		}

		book->release();
	}
}

void	Do_ExportReportToExcel()
{
	libxl::Book* book = xlCreateXMLBook();

	for (auto item : SETTINGS().ReportData) {
		const GS::UniString _k = *item.key;
		libxl::Sheet* sheet = book->addSheet(UNISTR_TO_LIBXLSTR(_k));
		sheet->setCol(0, 1, 50.0);

		GS::Array<GS::UniString> titles;
		if (SETTINGS().ReportHeaderS.ContainsKey(*item.key))
			titles = SETTINGS().ReportHeaderS[*item.key];
		else
			titles = { "Object", "Number" };

		for (UIndex ii = 0; ii < titles.GetSize(); ++ii) {
			sheet->writeStr(0, ii, UNISTR_TO_LIBXLSTR(titles[ii]));
		}

		UIndex ii = 1;

		for (auto iitem : *item.value)
		{
			if (iitem.value || SETTINGS().CheckBoxData[ZERO_CHECKBOX])
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

