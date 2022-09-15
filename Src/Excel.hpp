#ifndef _EXCEL_HPP
#define _EXCEL_HPP

#include	"APIEnvir.h"
#include	"ACAPinc.h"					// also includes APIdefs.h
#include	"APICommon.h"
#include	"FileSystem.hpp"
#include	"DGFileDialog.hpp"
#include	"DataStructs.hpp"


// -----------------------------------------------------------------------------
// Open the selected XLSX file into a library part
// -----------------------------------------------------------------------------

bool	GetOpenFile(IO::Location* dloc,
	const char* fileExtensions,
	const GS::UniString& filterText,
	DG::FileDialog::Type i_type /*= DG::FileDialog::Type::OpenFile*/);

// -----------------------------------------------------------------------------
// Export GUID and a few basic parameters of all walls placed in the current project.
// -----------------------------------------------------------------------------

void	Do_ImportNamesFromExcel(CntlDlgData& io_cntlDlgData);

void	Do_ExportReportToExcel(CntlDlgData& io_cntlDlgData);

#endif //_EXCEL_HPP