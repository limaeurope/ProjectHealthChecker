#ifndef LIBXL_EXTENDED_HPP
#define LIBXL_EXTENDED_HPP

#include	"APIEnvir.h"
#include	"ACAPinc.h"					// also includes APIdefs.h
#include	"APICommon.h"
#include	"LibXL/libxl.h"
#include	"ReportRow.hpp"


using namespace libxl;

struct SheetExtended : public Sheet
{
	SheetExtended(ISheetT<wchar_t>*) {};
	SheetExtended(SheetExtended*) {};

	//virtual             bool XLAPIENTRY writeNum(int row, int col, double value, IFormatT<wchar_t>* format = 0) = 0;
	bool XLAPIENTRY writeNum(int row, int col, double value, IFormatT<wchar_t>* format = 0) { return Sheet::writeNum(row, col, value, format); };
	bool XLAPIENTRY writeNum(int row, int col, GS::Array<UInt32> valueS, IFormatT<wchar_t>* format = 0);
	bool XLAPIENTRY writeNum(int row, int col, ReportRow valueS, IFormatT<wchar_t>* format = 0);
};

template<class T>
struct _BookExtended : public libxl::IBookT<wchar_t>
{
	//ISheetT<wchar_t>* XLAPIENTRY addSheet(const wchar_t* name, ISheetT<wchar_t>* initSheet = 0) = 0;
	SheetExtended* XLAPIENTRY addSheet(const wchar_t* name, Sheet* initSheet = 0);

	_BookExtended(Book*) {};
};

typedef _BookExtended<wchar_t> BookExtended;
#endif // LIBXL_EXTENDED_HPP

