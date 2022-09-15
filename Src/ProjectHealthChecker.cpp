// *****************************************************************************
//
// Namespaces:        Contact person: samuel.karli@limaeurope.com
//     -None-
//
// *****************************************************************************

#define	_ELEMENT_TEST_TRANSL_


// ---------------------------------- Includes ---------------------------------

#include	"APIEnvir.h"
#include	"ACAPinc.h"					// also includes APIdefs.h
#include	"APICommon.h"

#include	"ProjectHealthChecker.hpp"


// ---------------------------------- Types ------------------------------------

// ---------------------------------- Variables --------------------------------

static CntlDlgData			cntlDlgData{1};
static APITypeDict			apiTypeDict;
static GS::HashTable<GS::UniString, UInt32> iLibPartInstanceS{};

#define OK_BUTTON			1
#define SOURCE_GROUP_POPUP	2
#define EXPORT_BUTTON		3

#define ZERO_CHECKBOX		2
#define IMPORT_BUTTON		3

// ----------------------------------  -------------------------------

static bool hasTexture(API_Attribute i_apiAttrib, AbstractData* i_attrs)
{
	return i_apiAttrib.material.texture.fileLoc != NULL;
}

static bool nameContains(API_Attribute i_apiAttrib, AbstractData* i_attrs)
{
	return GS::UniString(i_apiAttrib.header.name)
		.FindFirst(static_cast<StringData*>(i_attrs)->string) < MaxUIndex;
}

static DataObject* getTextureSize(API_Attribute i_apiAttrib)
{
	if (i_apiAttrib.material.texture.status == 0) 
		throw 1;
	IO::Location loc{ *i_apiAttrib.material.texture.fileLoc };
	IO::File f{loc};
	GSErrCode err;
	GS::UniString path = "";
	UInt64 fileSize = 0;

	if (!loc.IsEmpty())
	{
		FileSizeReportObject* result = new FileSizeReportObject;
		
		err = loc.ToPath(&path);
		if (err) throw err;
		
		result->path = path;
		if (path.Contains("\\"))
			result->name = path(path.FindLast("\\") + 1, path.GetLength() - path.FindLast("\\") - 1);

		err = f.GetDataLength(&fileSize);
		if (err) throw err;

		result->size = fileSize;

		return result;
	}
	else throw 1;
}

// ----------------------------------  -------------------------------

static void AddSum(GS::UniString i_sTable ) {
	//TODO
}

static void AddItem(GS::UniString i_sTable, GS::UniString i_sItem, UInt32 i_iItemNumber)
{
	// Adds an item to both the UI report and the .xlsx output
	if (!i_iItemNumber && !cntlDlgData.iAddZeroValues) return;

	char sItem[256], _sNumberOfWalls[256], sInt[256];

	//sprintf(sItem, "Number of %s", i_sItem.ToCStr().Get());
	itoa(i_iItemNumber, sInt, 10);

	if (!cntlDlgData.reportData.ContainsKey(i_sTable))
	{ 
		ReportData _rd{};
		cntlDlgData.reportData.Add(i_sTable, _rd);
	}

	cntlDlgData.reportData[i_sTable].Add(i_sItem, i_iItemNumber);

	sprintf(_sNumberOfWalls, "%s: %s", i_sItem.ToCStr().Get(), sInt);
	DGListInsertItem(32400, 2, DG_LIST_BOTTOM);
	DGListSetItemText(32400, 2, DG_LIST_BOTTOM, GS::UniString(_sNumberOfWalls));
}

static void AddList(GS::UniString i_sTable, GS::UniString i_sItem, UInt32 i_iItemNumber)
{

	if (!cntlDlgData.reportData.ContainsKey(i_sTable))
	{
		ReportData _rd{};
		cntlDlgData.reportData.Add(i_sTable, _rd);
	}

	cntlDlgData.reportData[i_sTable].Add(i_sItem, i_iItemNumber);
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

		DGSetItemValLong(dialID, ZERO_CHECKBOX, cntlDlgData.iAddZeroValues);


		GS::Array<DataObject*> lLibParts;

		lLibParts = ListLibParts(apiTypeDict);

		CountLibPartInstances(iLibPartInstanceS);

		for (auto libPart : iLibPartInstanceS)
		{
			AddItem("Library Part Instances", *libPart.key, *libPart.value);
		}

		GS::Array<FileSizeReportObject> aEmbedded, aSpecial, aNormal;

		for (DataObject* lp : lLibParts)
		{
			FileSizeReportObject* _lp = (FileSizeReportObject*)lp;
			if (_lp->libType == API_BuiltInLibrary)
				aSpecial.Push(*_lp);

			if (_lp->libType == API_LocalLibrary
				|| _lp->libType == API_ServerLibrary)
				aNormal.Push(*_lp);

			if (_lp->libType == API_EmbeddedLibrary)
				aEmbedded.Push(*_lp);

			delete lp;
		}

		for (auto item : aNormal)
			AddItem("LibPart data", item.name, (UInt32)item.size);

		for (auto item : aEmbedded)
			AddItem("Embedded LibPart data", item.name, (UInt32)item.size);


		for (UINT16 i = 1; i <= ac_types.GetSize(); i++)
		{
			GS::Array<API_Guid> _array{};
			char intStr[256], _sNumberOfWalls[256], _sNumberOfWalls2[256];

			err = ACAPI_Element_GetElemList(static_cast<API_ElemTypeID>(i), &_array);

			auto _a = ac_types[i-1].ToCStr().Get();
			sprintf(_sNumberOfWalls2, "Number of %s", _a);
			AddItem("Elements", _sNumberOfWalls2, _array.GetSize());
		}

		AddItem("SEO Data", "Number of SEO Operators/Targets", GetSEOElements().GetSize());
		AddItem("SEO Data", "Number of erroneous SEO Operators/Targets", GetSEOElements(true).GetSize());


		short iNavItems = 0;

		for (UInt16 iMT = 1; iMT < ac_mapTypes.GetSize(); iMT++)
			for (UInt16 iNIT = 1; iNIT < ac_navItemTypes.GetSize(); iNIT++)
			{
				iNavItems = GetNavigatorItems(static_cast<API_NavigatorMapID>(iMT), static_cast<API_NavigatorItemTypeID>(iNIT));

				if (iNavItems > 0 || cntlDlgData.iAddZeroValues)
				{
					AddItem(ac_mapTypes[iMT], ac_navItemTypes[iNIT], iNavItems);
				}

				// -------------------------------------

				iNavItems = GetNavigatorItems(static_cast<API_NavigatorMapID>(iMT), static_cast<API_NavigatorItemTypeID>(iNIT), "Story");

				if (iNavItems > 0 || cntlDlgData.iAddZeroValues)
				{
					AddItem(ac_mapTypes[iMT], ac_navItemTypes[iNIT] + "Story", iNavItems);
				}
			}

		AddItem("Layer data", "Number of Layers", CountAttributes(API_LayerID));

		for (auto sFilter: cntlDlgData.filterStrings)
		{ 
			auto iCount = CountAttributes(API_LayerID, nameContains, &StringData{ sFilter });
			AddItem("Layer data", "Number of Layers containing the string \"" + sFilter + "\"", iCount);
		}
		
		AddItem("Layer data", "Number of Materials", CountAttributes(API_MaterialID));
		AddItem("Layer data", "Number of Materials with Texture", CountAttributes(API_MaterialID, hasTexture));

		GS::Array<DataObject*> lTextures;
		
		lTextures = ListAttributes(API_MaterialID, getTextureSize);

		for (DataObject* tex : lTextures)
		{
			FileSizeReportObject* _tex = (FileSizeReportObject*)tex;
			AddItem("Texture data", _tex->name, (UInt16)_tex->size);
			delete tex;
		}

		Int32 iLibParts;


		break;
	}
	case DG_MSG_CLICK:
		switch (item) {
		case OK_BUTTON:
			result = item;
			break;
		case EXPORT_BUTTON:
			Do_ExportReportToExcel(cntlDlgData);

			result = item;
			break;
		}
		break;
	}

	return result;
}

static short DGCALLBACK SettingsDlgCallBack(short message, short dialID, short item, DGUserData userData, DGMessageData msgData)
{
	short result = 0;
	GS::UniString _text{};
	API_PropertyDefinition _def;

	switch (message) {
	case DG_MSG_INIT:
	{
		GSErrCode err;

		DGSetItemValLong(dialID, ZERO_CHECKBOX, cntlDlgData.iAddZeroValues);

		break;
	}
	case DG_MSG_CLICK:
		switch (item) {
		case OK_BUTTON:
			result = item;
			break;
		case IMPORT_BUTTON:
			Do_ImportNamesFromExcel(cntlDlgData);

			result = item;
			break;
		}
	case DG_MSG_CHANGE:
		switch (item) {
		case ZERO_CHECKBOX:
			//TODO Refresh the dialog
			cntlDlgData.iAddZeroValues = DGGetItemValLong(dialID, ZERO_CHECKBOX);
			break;
		}
		break;
	}

	return result;
}

static GSErrCode	Do_Report()
{
	GSErrCode		err = NoError;

	err = DGModalDialog(ACAPI_GetOwnResModule(), 32400, ACAPI_GetOwnResModule(), CntlDlgCallBack, (DGUserData)&cntlDlgData);
	ACAPI_KeepInMemory(true);

	return err;
}

static GSErrCode	Do_Settings()
{
	GSErrCode		err = NoError;

	err = DGModalDialog(ACAPI_GetOwnResModule(), 32401, ACAPI_GetOwnResModule(), SettingsDlgCallBack, (DGUserData)&cntlDlgData);
	ACAPI_KeepInMemory(true);

	return err;
}		// Do_Report


// -----------------------------------------------------------------------------
// Elements: Solid Operations Functions
// -----------------------------------------------------------------------------

GSErrCode __ACENV_CALL ProjectHealthChecker (const API_MenuParams *menuParams)
{
	apiTypeDict = APITypeDict{};

	return ACAPI_CallUndoableCommand ("Element Test API Function",
		[&] () -> GSErrCode {

			switch (menuParams->menuItemRef.itemIndex) {
				case 1:		Do_Report();						break;
				case 2:		Do_Settings();						break;
				default:										break;
			}

			return NoError;
		});
}		/* ProjectHealthChecker */


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

	err = ACAPI_Install_MenuHandler (32506, ProjectHealthChecker);

	return err;
}		/* Initialize */


// -----------------------------------------------------------------------------
// Called when the Add-On is going to be unloaded
// -----------------------------------------------------------------------------
GSErrCode __ACENV_CALL	FreeData (void)
{
	return NoError;
}		/* FreeData */

