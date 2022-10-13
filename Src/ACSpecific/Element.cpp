
#include	"Element.hpp"
#include	"ACTypes.hpp"
#include	"../Data/SettingsSingleton.hpp"


void ProcessElements()
{
	SETTINGS().GetSheet("Elements").SetHeader(GS::Array<GS::UniString>{ "Object type", "Number of placed instances" });

	for (UINT16 i = 1; i <= ac_types.GetSize(); i++)
	{
		GS::Array<API_Guid> _array{};
		char intStr[256], _sNumberOfWalls[256], _sNumberOfWalls2[256];

		GSErrCode err = ACAPI_Element_GetElemList(static_cast<API_ElemTypeID>(i), &_array);

		auto _a = ac_types[i - 1].ToCStr().Get();
		GS::UniString sNumberOfElements = GS::UniString("Number of ") + _a;
		SETTINGS().GetSheet("Elements").AddItem(sNumberOfElements, _array.GetSize());
	}
}

