#include	"Excel.hpp"
#include	"LibXL/libxl.h"
#include	"LibXLExtended.hpp"
#include	"SettingsSingleton.hpp"
#define UNISTR_TO_LIBXLSTR(str) (str.ToUStr ())


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

