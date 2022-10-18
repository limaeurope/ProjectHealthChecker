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

	bool XLAPIENTRY writeNum(int row, int col, double value, IFormatT<wchar_t> * format = 0) { return Sheet::writeNum(row, col, value, format); };
	bool XLAPIENTRY writeNum(int row, int col, const GS::Array<UInt32> &valueS, IFormatT<wchar_t>* format = 0);
	bool XLAPIENTRY writeNum(int row, int col, ResultRow &valueS, IFormatT<wchar_t>* format = 0);
};

template<class T>
struct _BookExtended : public libxl::IBookT<wchar_t>
{
	SheetExtended* XLAPIENTRY addSheet(const wchar_t* name, Sheet* initSheet = 0);
	SheetExtended* XLAPIENTRY _getSheet(const GS::UniString& i_sSheet) const;
	SheetExtended* XLAPIENTRY _getSheet(const wchar_t* const i_sSheet) const;

	_BookExtended(Book*) {};
};

typedef _BookExtended<wchar_t> BookExtended;

template <class T>
SheetExtended* XLAPIENTRY _BookExtended<T>::addSheet(const wchar_t* name, Sheet* initSheet)
{
	//auto _s = (Sheet*)initSheet;
	auto _r = Book::addSheet(name, initSheet);
	return (SheetExtended*)_r;
}

#endif // LIBXL_EXTENDED_HPP

