#include	"Navigator.hpp"
#include	"DataStructs.hpp"
#include	"acTypes.hpp"
#include	"SettingsSingleton.hpp"


void ProcessNavigatorItems()
{
	short iNavItems = 0;

	for (UInt16 iMT = 1; iMT < ac_mapTypes.GetSize(); iMT++)
		for (UInt16 iNIT = 1; iNIT < ac_navItemTypes.GetSize(); iNIT++)
		{
			iNavItems = GetNavigatorItems(static_cast<API_NavigatorMapID>(iMT), static_cast<API_NavigatorItemTypeID>(iNIT));

			if (iNavItems > 0 || SettingsSingleton::GetInstance().CheckBoxData[ZERO_CHECKBOX])
			{
				AddItem(ac_mapTypes[iMT], ac_navItemTypes[iNIT], iNavItems);
			}

			// -------------------------------------

			iNavItems = GetNavigatorItems(static_cast<API_NavigatorMapID>(iMT), static_cast<API_NavigatorItemTypeID>(iNIT), "Story");

			if (iNavItems > 0 || SettingsSingleton::GetInstance().CheckBoxData[ZERO_CHECKBOX])
			{
				AddItem(ac_mapTypes[iMT], ac_navItemTypes[iNIT] + "Story", iNavItems);
			}
		}
}


short GetChildrenNumber(API_NavigatorItem i_item,
	const API_NavigatorItemTypeID& i_navID,
	const GS::UniString& i_sInExclude /*= ""*/,
	const bool i_isInclude /*= true*/)
{
	GSErrCode err;
	short result = 0;
	GS::Array<API_NavigatorItem> childItems;

	auto _name = GS::UniString(i_item.uName).ToCStr().Get();

	err = ACAPI_Navigator(APINavigator_GetNavigatorChildrenItemsID, &i_item, nullptr, &childItems);

	if (i_item.itemType == i_navID)
		if (i_sInExclude.GetLength())
			if (i_isInclude == (GS::UniString(i_item.uName).FindFirst(i_sInExclude) < MaxUSize))
				result += 1;
			else
				result += 1;

	for (const API_NavigatorItem& childItem : childItems)
	{
		result += GetChildrenNumber(childItem, i_navID, i_sInExclude, i_isInclude);
	}

	return result;
}


short GetNavigatorItems(const API_NavigatorMapID& i_mapID,
	const API_NavigatorItemTypeID& i_navID,
	const GS::UniString& i_inExcludeString /*= ""*/,
	const bool i_isInclude /*= true*/)
{
	short result = 0;
	GSErrCode err;

	GS::Array<API_NavigatorItem> childItems;
	API_NavigatorItem item = {};
	API_NavigatorSet	set;
	GS::Array<API_NavigatorSet>	sets;
	BNZeroMemory(&set, sizeof(API_NavigatorSet));
	set.mapId = i_mapID;

	if (i_mapID != API_PublisherSets)
	{
		err = ACAPI_Navigator(APINavigator_GetNavigatorSetID, &set, nullptr);
		sets.Push(set);
	}
	else
	{
		Int32 nSet = 0;
		err = ACAPI_Navigator(APINavigator_GetNavigatorSetNumID, &nSet, nullptr);

		for (Int32 _i = 0; _i < nSet; _i++)
		{
			err = ACAPI_Navigator(APINavigator_GetNavigatorSetID, &set, &_i);
			sets.Push(set);
		}
	}

	for (const auto& _set : sets)
	{
		item.guid = set.rootGuid;
		result += GetChildrenNumber(item, i_navID, i_inExcludeString, i_isInclude);
	}

	return result;
}

