#ifndef _NAVIGATOR_HPP
#define _NAVIGATOR_HPP

#include	"../APIEnvir.h"
#include	"ACAPinc.h"					// also includes APIdefs.h
#include	"../APICommon.h"

void ProcessNavigatorItems();

short GetChildrenNumber(API_NavigatorItem* const i_item,
	const API_NavigatorItemTypeID& i_navID,
	const GS::UniString& i_sInExclude = "",
	const bool i_isInclude = true);

short GetNavigatorItems(const API_NavigatorMapID& i_mapID,
	const API_NavigatorItemTypeID& i_navID,
	const GS::UniString& i_inExcludeString = "",
	const bool i_isInclude = true);

#endif	//_NAVIGATOR_HPP

