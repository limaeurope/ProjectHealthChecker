#include	"AttributeUsage.hpp"


AttributeUsage::AttributeUsage()
{
	API_Element         element;

	GS::Array<API_Guid> elemList;
	GSErrCode err = ACAPI_Element_GetElemList(API_ZombieElemID, &elemList);

	for (auto e : elemList)
	{
		BNZeroMemory(&element, sizeof(API_Element));
		element.header.guid = e;
		err = ACAPI_Element_Get(&element);
		if (err) throw err;

		if (!layerContentTable.ContainsKey(element.header.layer))
		{
			layerContentTable.Put(element.header.layer, 1);
		}
		else
			layerContentTable[element.header.layer]++;
	}
}