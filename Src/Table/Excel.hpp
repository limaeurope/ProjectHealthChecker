#ifndef _EXCEL_HPP
#define _EXCEL_HPP

#include	"../APIEnvir.h"
#include	"ACAPinc.h"					// also includes APIdefs.h
#include	"../APICommon.h"
#include	"FileSystem.hpp"
#include	"DGFileDialog.hpp"
#include	"../Data/DataStructs.hpp"


// -----------------------------------------------------------------------------
// Open the selected XLSX file into a library part
// -----------------------------------------------------------------------------

bool	GetOpenFile(IO::Location* const dloc,
	const char* const fileExtensions,
	const GS::UniString& filterText,
	const DG::FileDialog::Type i_type /*= DG::FileDialog::Type::OpenFile*/);

// -----------------------------------------------------------------------------
// Export GUID and a few basic parameters of all walls placed in the current project.
// -----------------------------------------------------------------------------

void	Do_ImportNamesFromExcel();

#endif //_EXCEL_HPP