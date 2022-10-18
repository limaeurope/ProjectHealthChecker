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
		if (SETTINGS().CheckBoxData[Zero_checkbox] || _v)
			bSuccess &= writeNum(row, col + _k, _v, format);
	}

	return bSuccess;
}

SheetExtended* XLAPIENTRY _BookExtended<wchar_t>::_getSheet(const GS::UniString& i_sSheet) const
{
	for (int i = 0; i < sheetCount(); i++)
	{
		if (i_sSheet == GS::UniString(getSheetName(i)))
		{
			auto _i = getSheet(i);
			return (SheetExtended*) _i;
		}
	}

	return nullptr;
}

SheetExtended* XLAPIENTRY _BookExtended<wchar_t>::_getSheet(const wchar_t * const i_sSheet) const
{
	return _getSheet(GS::UniString(i_sSheet));
};

