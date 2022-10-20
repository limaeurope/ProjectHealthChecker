
#include	"Element.hpp"
#include	"ACTypes.hpp"
#include	"../Data/SettingsSingleton.hpp"
#include	"../Enums/StringsEnum.hpp"


void ProcessElements()
{
	SETTINGS().GetSheet(Elements).SetHeader(GS::Array<int>{ ObjType, NumOfPlacedInstances});

	for (UINT16 i = 1; i <= ac_types.GetSize(); i++)
	{
		GS::Array<API_Guid> _array{};

		GSErrCode err = ACAPI_Element_GetElemList(static_cast<API_ElemTypeID>(i), &_array);

		auto _acType = ac_types[i - 1].ToCStr().Get();
		GS::UniString sNumberOfElements = GS::UniString("Number of ") + _acType;
		SETTINGS().GetSheet(Elements).AddItem(sNumberOfElements, _array.GetSize());
	}
}

