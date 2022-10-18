#include	"APIEnvir.h"
#include	"ACAPinc.h"					// also includes APIdefs.h
#include	"APICommon.h"

#include	"Excel.hpp"
#include	"LibXL/libxl.h"
#include	"LibXLExtended.hpp"
#include	"SettingsSingleton.hpp"
#include	"LibXLExtended.hpp"

#define UNISTR_TO_LIBXLSTR(str) (str.ToUStr ())

void	Do_ImportNamesFromExcel()
{
	IO::Location xlsFileLoc;
	if (!GetOpenFile(&xlsFileLoc, "xlsx", "*.xlsx", DG::FileDialog::OpenFile))
		return;

	GS::UniString filepath;
	xlsFileLoc.ToPath(&filepath);

	BookExtended* book;
	bool isBookLoaded = false;

	book = (BookExtended*)xlCreateBook();
	if (book->load(filepath.ToUStr()))
		isBookLoaded = true;
	else
	{
		book = (BookExtended*)xlCreateXMLBook();
		if (book->load(filepath.ToUStr()))
			isBookLoaded = true;
	}

	if (isBookLoaded)
	{
		if (SheetExtended* sheet = book->_getSheet("ListLayers"))
		{
			for (int row = sheet->firstRow(); row < sheet->lastRow(); ++row)
			{
				if (const wchar_t* sFilter = sheet->readStr(row, 0))
					SETTINGS().FilterStrings.Add(GS::UniString(sFilter));
			}
		}

		if (SheetExtended* sheet = book->_getSheet("CountLayers"))
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

