#include	"LibXLExtended.hpp"
#include	"SettingsSingleton.hpp"


using namespace libxl;

bool XLAPIENTRY SheetExtended::writeNum(const int row, const int col, const GS::Array<UInt32> &valueS, IFormatT<wchar_t>* format)
{
	bool bSuccess = true;
	for (UInt16 i = 0; i < valueS.GetSize(); i ++)
		bSuccess &= writeNum(row, col + i, (double)valueS[i], format);
	return bSuccess;
}

bool XLAPIENTRY SheetExtended::writeNum(const int row, const int col, ResultRow&valueS, IFormatT<wchar_t>* format)
{
	bool bSuccess = true;

	for (auto i = valueS.Begin(); i != valueS.End(); i++)
	{
		auto _k = *(*i).key;
		auto _v = *(*i).value;
		if (SETTINGS().CheckBoxData[ZERO_CHECKBOX] || _v)
			bSuccess &= writeNum(row, col + _k, _v, format);
	}

	return bSuccess;
}

template <class T>
SheetExtended* XLAPIENTRY _BookExtended<T>::addSheet(const wchar_t* name, Sheet* initSheet)
{
	auto _s = (Sheet*)initSheet;
	auto _r = Book::addSheet(name, _s);
	return (SheetExtended*)_r;
}