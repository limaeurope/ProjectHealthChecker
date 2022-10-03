
#include	"Element.hpp"
#include	"ACTypes.hpp"
#include	"Table.hpp"


void ProcessElements()
{
	SetHeader("Elements", ReportDataHeader{ "Object type", "Number of placed instances" });

	for (UINT16 i = 1; i <= ac_types.GetSize(); i++)
	{
		GS::Array<API_Guid> _array{};
		char intStr[256], _sNumberOfWalls[256], _sNumberOfWalls2[256];

		GSErrCode err = ACAPI_Element_GetElemList(static_cast<API_ElemTypeID>(i), &_array);

		auto _a = ac_types[i - 1].ToCStr().Get();
		//sprintf(_sNumberOfWalls2, "Number of %s", _a);
		GS::UniString sNumberOfElements = GS::UniString("Number of ") + _a;
		AddItem("Elements", sNumberOfElements, _array.GetSize());
	}
}

