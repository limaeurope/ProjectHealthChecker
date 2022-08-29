// *****************************************************************************
// *****************************************************************************

#define	_ELEMENT_TEST_TRANSL_


// ---------------------------------- Includes ---------------------------------

#include	"APIEnvir.h"
#include	"ACAPinc.h"					// also includes APIdefs.h
#include	"APICommon.h"

#include	"ProjectHealthChecker.h"
#include	"DG.h"
#include	"ACAPinc.h"
#include	"FileSystem.hpp"
#include	"LibXL/libxl.h"
#include	"DGFileDialog.hpp"


// ---------------------------------- Types ------------------------------------

typedef GS::HashTable<GS::UniString, UInt16> ReportData;

typedef struct {
	GS::HashTable<GS::UniString, ReportData> reportData;
	Int64 iSourceGroup;
	Int64 iTargetGroup;
	Int64 iSource;
	Int64 iTarget;
	Int32 iAppend;
} CntlDlgData;

// ---------------------------------- Variables --------------------------------

static CntlDlgData			cntlDlgData;
#define OK_BUTTON			1
#define SOURCE_GROUP_POPUP	2
#define EXPORT_BUTTON		3

static const GS::Array<GS::UniString> ac_types{
	"Walls",
	"Columns",
	"Beams",
	"Windows",
	"Doors",
	"Objects",
	"Lamps",
	"Slabs",
	"Roofs",
	"Meshes",

	"Dimensions",
	"RadialDimensions",
	"LevelDimensions",
	"AngleDimensions",

	"Texts",
	"Labels",
	"Zones",

	"Hatches",
	"Lines",
	"PolyLines",
	"Arcs",
	"Circles",
	"Splines",
	"Hotspots",

	"CutPlanes",
	"Cameras",
	"CamSets",

	"Groups",
	"SectElems",

	"Drawings",
	"Pictures",
	"Details",
	"Elevations",
	"InteriorElevations",
	"Worksheets",

	"Hotlinks",

	"CurtainWalls",
	"CurtainWallSegments",
	"CurtainWallFrames",
	"CurtainWallPanels",
	"CurtainWallJunctions",
	"CurtainWallAccessorys",
	"Shells",
	"Skylights",
	"Morphs",
	"ChangeMarkers",
	"Stairs",
	"Risers",
	"Treads",
	"StairStructures",
	"Railings",
	"RailingToprails",
	"RailingHandrails",
	"RailingRails",
	"RailingPosts",
	"RailingInnerPosts",
	"RailingBalusters",
	"RailingPanels",
	"RailingSegments",
	"RailingNodes",
	"RailingBalusterSets",
	"RailingPatterns",
	"RailingToprailEnds",
	"RailingHandrailEnds",
	"RailingRailEnds",
	"RailingToprailConnections",
	"RailingHandrailConnections",
	"RailingRailConnections",
	"RailingEndFinishs",

	"AnalyticalSupports",
	"AnalyticalLinks",

	"BeamSegments",
	"ColumnSegments",
	"Openings",
	"AnalyticalPointLoads",
	"AnalyticalEdgeLoads",
	"AnalyticalSurfaceLoads",
};


static const GS::Array<GS::UniString> ac_mapTypes{
	"Undefined Map",
	"Project Map",
	"PublicView Map",
	"MyView Map",
	"Layout Map",
	"Publisher Sets",
};


static const GS::Array<GS::UniString> ac_navItemTypes{
	"Undefined",
	"Project",
	"Story",
	"Section",
	"DetailDrawing",
	"Perspective",
	"Axonometry",
	"List",
	"Schedule",
	"Toc",
	"Camera",
	"CameraSet",
	"Info",
	"Help",
	"Layout",
	"MasterLayout",
	"Book",
	"MasterFolder",
	"SubSet",
	"TextList",
	"Elevation",
	"InteriorElevation",
	"WorksheetDrawing",
	"DocumentFrom3D",
	"Folder",
	"Drawing",
};


// ---------------------------------- Prototypes -------------------------------

static void AddItem(GS::UniString i_sTable, GS::UniString i_sItem, UInt16 i_iItemNumber)
{
	char sItem[256], _sNumberOfWalls[256], sInt[256];

	sprintf(sItem, "Number of %s", i_sItem.ToCStr().Get());
	itoa(i_iItemNumber, sInt, 10);

	if (!cntlDlgData.reportData.ContainsKey(i_sTable))
	{ 
		ReportData _rd{};
		cntlDlgData.reportData.Add(i_sTable, _rd);
	}

	cntlDlgData.reportData[i_sTable].Add(i_sItem, i_iItemNumber);

	sprintf(_sNumberOfWalls, "Number of %s: %s", sItem, sInt);
	DGListInsertItem(32400, 2, DG_LIST_BOTTOM);
	DGListSetItemText(32400, 2, DG_LIST_BOTTOM, GS::UniString(_sNumberOfWalls));
}


static short GetChildrenNumber(API_NavigatorItem i_item)
{
	GSErrCode err;
	GS::Array<API_NavigatorItem> childItems;

	err = ACAPI_Navigator(APINavigator_GetNavigatorChildrenItemsID, &i_item, nullptr, &childItems);

	short result = childItems.GetSize();

	for (const API_NavigatorItem& childItem : childItems)
		result += GetChildrenNumber(childItem);

	return result;
}


static short GetNavigatorItems(API_NavigatorMapID i_mapID, API_NavigatorItemTypeID i_navID)
{
	short result = 0;
	GSErrCode err;

	// create drawing data
	//GS::Array<API_NavigatorItem> navItems;
	//API_NavigatorItem	navItem = {}, parentItem;

	//navItem.mapId = i_mapID;
	//navItem.itemType = i_navID;

	//err = ACAPI_Navigator(APINavigator_SearchNavigatorItemID, &navItem, nullptr, &navItems);

	//if (err != NoError)
	//	return -1;
	//else result = navItems.GetSize();

	////for (API_NavigatorItem& _item : navItems)
	//{
	GS::Array<API_NavigatorItem> childItems;
	API_NavigatorItem item = {};
	API_NavigatorSet	set;
	set.mapId = i_mapID;
	err = ACAPI_Navigator(APINavigator_GetNavigatorSetID, &set, 0);

	item.guid = set.rootGuid;

	result = GetChildrenNumber(item);

	//err = ACAPI_Navigator(APINavigator_GetNavigatorChildrenItemsID, &item, nullptr, &childItems);
	////}

	//for (auto childItem : childItems)
	//{
	//	GS::Array<API_NavigatorItem> navItems2;

	//	err = ACAPI_Navigator(APINavigator_GetNavigatorChildrenItemsID, &childItem, nullptr, &navItems2);
	//}



	//GSErrCode err = NoError;

	//API_NavigatorView   view;
	//API_NavigatorItem   parent;
	//API_NavigatorItem   item = {};
	//GS::Array<API_NavigatorItem> items;

	//item.itemType = i_navID;
	//item.mapId = i_mapID;

	//err = ACAPI_Navigator(APINavigator_SearchNavigatorItemID, &item, nullptr, &items);
	//if (err != NoError)
	//	return -1;

	//char    str[256];
	//Int32   num;

	//ACAPI_WriteReport("Stories of View Map:", false);

	//for (const API_NavigatorItem& navItem : items) {
	//	sprintf(str, "%s", GS::UniString(navItem.uName).ToCStr().Get());
	//	ACAPI_WriteReport(str, false);

	//	BNZeroMemory(&parent, sizeof(API_NavigatorItem));
	//	parent.mapId = navItem.mapId;
	//	err = ACAPI_Navigator(APINavigator_GetNavigatorParentItemID, nullptr, &parent, nullptr);
	//	sprintf(str, " - parent name: %s", GS::UniString(parent.uName).ToCStr().Get());
	//	ACAPI_WriteReport(str, false);

	//	err = ACAPI_Navigator(APINavigator_GetNavigatorChildNumID, &parent, &num, nullptr);
	//	sprintf(str, " - number of children of the parent: %d", num);
	//	ACAPI_WriteReport(str, false);

	//	BNZeroMemory(&view, sizeof(API_NavigatorView));
	//	err = ACAPI_Navigator(APINavigator_GetNavigatorViewID, nullptr, &view, nullptr);
	//	if (view.saveLaySet) {
	//		if (view.layerCombination[0] != 0)
	//			sprintf(str, " - Layer combination: %s", view.layerCombination);
	//		else
	//			sprintf(str, " - Layers individual");
	//	}
	//	else {
	//		sprintf(str, " - none layers");
	//	}
	//	BMhKill((GSHandle*)&view.layerStats);

	//	ACAPI_WriteReport(str, false);
	//}

	return result;
}

// -----------------------------------------------------------------------------
// Open the selected DWG file into a library part
// -----------------------------------------------------------------------------
static bool	GetOpenFile(IO::Location* dloc, const char* fileExtensions, const GS::UniString& filterText)
{
	if (dloc == nullptr)
		return false;

	FTM::TypeID	retID;
	FTM::FileTypeManager	ftman("MyManager");
	FTM::FileType			type(nullptr, fileExtensions, 0, 0, 0);
	FTM::TypeID				id = FTM::FileTypeManager::SearchForType(type);
	if (id == FTM::UnknownType)
		id = ftman.AddType(type);

	DG::FileDialog	dlg(DG::FileDialog::Save);			

	if (!dlg.Invoke())
		return false;

	*dloc = dlg.GetSelectedFile();
	return true;
}

// -----------------------------------------------------------------------------
// Export GUID and a few basic parameters of all walls placed in the current project.
// -----------------------------------------------------------------------------

#define UNISTR_TO_LIBXLSTR(str) (str.ToUStr ())

static void	Do_ExportReportToExcel(void)
{
	DBPrintf("Exporting walls to Excel document...\n");

	libxl::Book* book = xlCreateXMLBook();

	for (auto item : cntlDlgData.reportData) {
		const GS::UniString _k = *item.key;
		libxl::Sheet* sheet = book->addSheet(UNISTR_TO_LIBXLSTR(_k));
		sheet->setCol(0, 1, 100.0);

		GS::Array<GS::UniString> titles =
		{ "Object type", "Number" };

		for (UIndex ii = 0; ii < titles.GetSize(); ++ii) {
			sheet->writeStr(0, ii, UNISTR_TO_LIBXLSTR(titles[ii]));
		}

		UIndex ii = 1;

		for (auto iitem : *item.value)
		{

			sheet->writeStr(ii, 0, iitem.key->ToUStr());
			sheet->writeNum(ii++, 1, *iitem.value);
		}
	}


	//libxl::Font* reportFormatFont = book->addFont();
	//reportFormatFont->setSize(6);
	//reportFormatFont->setColor(libxl::COLOR_GRAY50);
	//libxl::Format* reportFormat = book->addFormat();
	//reportFormat->setFont(reportFormatFont);



	//UIndex ii = 1;
	//for (auto item : cntlDlgData.reportData) {
	//	sheet->writeStr(ii, 0, item.key->ToUStr());
	//	sheet->writeNum(ii++, 1, *item.value);
	//}

	IO::Location xlsFileLoc;
	if (!GetOpenFile(&xlsFileLoc, "xlsx", "*.xlsx"))
		return;

	GS::UniString filepath;
	xlsFileLoc.ToPath(&filepath);

	DBVERIFY(book->save(UNISTR_TO_LIBXLSTR(filepath)));
	book->release();

	DBPrintf("Export operation finished\n");
}

bool BoundingBoxesMatch(API_Element& element1, API_Element& element2)
{
	API_Box3D	bbox1, bbox2;
	GSErrCode				err;

	BNZeroMemory(&bbox1, sizeof(bbox1));
	BNZeroMemory(&bbox2, sizeof(bbox2));

	err = ACAPI_Database(APIDb_CalcBoundsID, &element1.header, &bbox1);
	err = ACAPI_Database(APIDb_CalcBoundsID, &element2.header, &bbox2);

	bool xMatch, yMatch, zMatch;
	xMatch = bbox1.xMax > bbox2.xMin && bbox2.xMax > bbox1.xMin;
	yMatch = bbox1.yMax > bbox2.yMin && bbox2.yMax > bbox1.yMin;
	zMatch = bbox1.zMax > bbox2.zMin && bbox2.zMax > bbox1.zMin;
	return xMatch && yMatch && zMatch;
}

GS::HashSet<API_Guid> GetSEOElements(bool isBoundingBoxConsidered = false)
{
	GSErrCode				err;
	API_SelectionInfo		selectionInfo;
	GS::Array<API_Neig>		selNeigs;
	API_Element				elementThis, elementOther;
	GS::Array<API_Guid>		guid_Targets, guid_Operators;
	API_Neig				_neig;
	GS::HashSet<API_Guid>		result;
	GS::Array<API_Guid>		_tempArray;

	err = ACAPI_Element_GetElemList(API_ZombieElemID, &_tempArray);

	for (const API_Guid& _guid : _tempArray) {
		bool bAdd = false;

		BNZeroMemory(&elementThis, sizeof(API_Element));
		elementThis.header.guid = _guid;
		err = ACAPI_Element_Get(&elementThis);
		if (err != NoError) {
			break;
		}

		err = ACAPI_Element_SolidLink_GetTargets(_guid, &guid_Targets);

		err = ACAPI_Element_SolidLink_GetOperators(_guid, &guid_Operators);

		for (const auto& guidTarget : guid_Targets) {
			BNZeroMemory(&elementOther, sizeof(API_Element));
			elementOther.header.guid = guidTarget;

			err = ACAPI_Element_Get(&elementOther);

			if (!isBoundingBoxConsidered || !BoundingBoxesMatch(elementThis, elementOther)) {
				result.Add(elementOther.header.guid);
				bAdd = true;
			}
		}

		for (const auto& guidOperator : guid_Operators) {
			BNZeroMemory(&elementOther, sizeof(API_Element));
			elementOther.header.guid = guidOperator;

			err = ACAPI_Element_Get(&elementOther);

			if (!isBoundingBoxConsidered || !BoundingBoxesMatch(elementThis, elementOther)) {
				result.Add(elementOther.header.guid);
				bAdd = true;
			}
		}

		if (bAdd)
			result.Add(_guid);
	}

	return result;
}

// -----------------------------------------------------------------------------
// Load a localisable Unicode string from resource
// -----------------------------------------------------------------------------

static void		GetStringFromResource(GS::UniString* buffer, short resID, short stringID)
{
	if (buffer != nullptr && !RSGetIndString(buffer, resID, stringID, ACAPI_GetOwnResModule()))
		buffer->Clear();

	return;
}		// GetStringFromResource

static short DGCALLBACK CntlDlgCallBack(short message, short dialID, short item, DGUserData userData, DGMessageData msgData)
{
	short result = 0;
	GS::UniString _text{};
	API_PropertyDefinition _def;

	switch (message) {
	case DG_MSG_INIT:
	{
		GSErrCode err;

		//for (UINT16 i = 1; i <= ac_types.GetSize(); i++)
		//{
		//	GS::Array<API_Guid> _array{};
		//	char intStr[256], _sNumberOfWalls[256], _sNumberOfWalls2[256];

		//	err = ACAPI_Element_GetElemList(static_cast<API_ElemTypeID>(i), &_array);
		//	itoa(_array.GetSize(), intStr, 10);
		//	auto _a = ac_types[i-1].ToCStr().Get();

		//	sprintf(_sNumberOfWalls2, "Number of %s", _a);
		//	cntlDlgData.reportData.Add(GS::UniString(_sNumberOfWalls2), _array.GetSize());

		//	sprintf(_sNumberOfWalls, "Number of %s: %s", _a,  intStr);
		//	DGListInsertItem(32400, 2, DG_LIST_BOTTOM);
		//	DGListSetItemText(32400, 2, DG_LIST_BOTTOM, GS::UniString(_sNumberOfWalls));
		//}

		//cntlDlgData.reportData.Add("Number of SEO Operators/Targets", GetSEOElements().GetSize());
		//cntlDlgData.reportData.Add("Number of erroneous SEO Operators/Targets", GetSEOElements(true).GetSize());


		short iNavItems = 0;

		for (UInt16 iMT = 0; iMT < ac_mapTypes.GetSize(); iMT++)
			for (UInt16 iNIT = 0; iNIT < ac_navItemTypes.GetSize(); iNIT++)
			{
				iNavItems = GetNavigatorItems(static_cast<API_NavigatorMapID>(iMT), static_cast<API_NavigatorItemTypeID>(iNIT));

				if (iNavItems != -1)
				{
					AddItem(ac_mapTypes[iMT], ac_navItemTypes[iNIT], iNavItems);
				}
			}


		break;
	}
	case DG_MSG_CLICK:
		switch (item) {
		case OK_BUTTON:
			result = item;
			break;
		case EXPORT_BUTTON:
			Do_ExportReportToExcel();

			result = item;
			break;
		}
	case DG_MSG_CHANGE:
		break;
	}

	return result;
}

static GSErrCode	Do_Report()
{
	GSErrCode		err = NoError;

	err = DGModalDialog(ACAPI_GetOwnResModule(), 32400, ACAPI_GetOwnResModule(), CntlDlgCallBack, (DGUserData)&cntlDlgData);
	
	return err;
}		// Do_Report


// -----------------------------------------------------------------------------
// Elements: Solid Operations Functions
// -----------------------------------------------------------------------------

GSErrCode __ACENV_CALL ElementsSolidOperation (const API_MenuParams *menuParams)
{
	return ACAPI_CallUndoableCommand ("Element Test API Function",
		[&] () -> GSErrCode {

			switch (menuParams->menuItemRef.itemIndex) {
				case 1:		Do_Report();						break;
				default:										break;
			}

			return NoError;
		});
}		/* ElementsSolidOperation */


// -----------------------------------------------------------------------------
// Dependency definitions
// -----------------------------------------------------------------------------
API_AddonType __ACENV_CALL	CheckEnvironment (API_EnvirParams* envir)
{
	RSGetIndString (&envir->addOnInfo.name, 32000, 1, ACAPI_GetOwnResModule ());
	RSGetIndString (&envir->addOnInfo.description, 32000, 2, ACAPI_GetOwnResModule ());

	return APIAddon_Preload;
}		/* RegisterAddOn */


// -----------------------------------------------------------------------------
// Interface definitions
// -----------------------------------------------------------------------------
GSErrCode __ACENV_CALL	RegisterInterface (void)
{
	GSErrCode	err;

	//
	// Register menus
	//
	err = ACAPI_Register_Menu (32506, 0, MenuCode_UserDef, MenuFlag_Default);

	return err;
}		/* RegisterInterface */


// -----------------------------------------------------------------------------
// Called when the Add-On has been loaded into memory
// to perform an operation
// -----------------------------------------------------------------------------
GSErrCode __ACENV_CALL	Initialize (void)
{
	GSErrCode err = NoError;

	//
	// Install menu handler callbacks
	//

	err = ACAPI_Install_MenuHandler (32506, ElementsSolidOperation);

	return err;
}		/* Initialize */


// -----------------------------------------------------------------------------
// Called when the Add-On is going to be unloaded
// -----------------------------------------------------------------------------
GSErrCode __ACENV_CALL	FreeData (void)
{
	return NoError;
}		/* FreeData */

