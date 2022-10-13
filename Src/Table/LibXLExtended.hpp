#ifndef LIBXL_EXTENDED_HPP
#define LIBXL_EXTENDED_HPP

#include	"APIEnvir.h"
#include	"ACAPinc.h"					// also includes APIdefs.h
#include	"APICommon.h"
#include	"LibXL/libxl.h"
#include	"ResultRow.hpp"


using namespace libxl;

struct SheetExtended : public Sheet
{
	SheetExtended(ISheetT<wchar_t>*) {};
	SheetExtended(SheetExtended*) {};

	bool XLAPIENTRY writeNum(const int row, const int col, const double value, IFormatT<wchar_t> * format = 0) { return Sheet::writeNum(row, col, value, format); };
	bool XLAPIENTRY writeNum(const int row, const int col, const GS::Array<UInt32> &valueS, IFormatT<wchar_t>* format = 0);
	bool XLAPIENTRY writeNum(const int row, const int col, ResultRow &valueS, IFormatT<wchar_t>* format = 0);
};

template<class T>
struct _BookExtended : public libxl::IBookT<wchar_t>
{
	SheetExtended* XLAPIENTRY addSheet(const wchar_t* name, Sheet* initSheet = 0);

	_BookExtended(Book*) {};
};

typedef _BookExtended<wchar_t> BookExtended;
#endif // LIBXL_EXTENDED_HPP

