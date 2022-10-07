#ifndef _TABLE_HPP
#define _TABLE_HPP

#include	"../APIEnvir.h"
#include	"ACAPinc.h"					// also includes APIdefs.h
#include	"../APICommon.h"
#include	"../Data/DataStructs.hpp"
#include	"DGFileDialog.hpp"

void SetHeader(const GS::UniString& i_sTable, const ReportDataHeader& reportDataHeader);

void AddItem(const GS::UniString& i_sTable, 
	const GS::UniString& i_sItem, 
	const UInt32 i_iItemNumber,
	const UInt16 i_col = 0);
#endif //_TABLE_HPP

