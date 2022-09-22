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
//#include	"Table/ResultTable.hpp"
//#include	"Table/Table.hpp"
#include	"DataStructs.hpp"
#include	"APITypeDict.hpp"
#include	"WinReg.hpp"
#include	"ACSpecific/Element.hpp"
#include	"ACSpecific/SEO.hpp"
#include	"ACSpecific/LibPart.hpp"
#include	"ACSpecific/Attribute.hpp"
#include	"ACSpecific/Navigator.hpp"
#include	"ACSpecific/Profile.hpp"
#include	"Table/Excel.hpp"

// ---------------------------------- Types ------------------------------------

// ---------------------------------- Variables --------------------------------

static CntlDlgData							cntlDlgData{ 1, {0, 0, 1, 1, 1, 1, 1, 1, 1} };
static APITypeDict							apiTypeDict;
static GS::HashTable<GS::UniString,	UInt32> iLibPartInstanceS{};
static ResultTable							resultTable;



// ----------------------------------  -------------------------------

void InitUI()
{
	cntlDlgData.CheckBoxData[LIBPART_CHECKBOX] = GetRegInt("LibraryPartData");
	cntlDlgData.CheckBoxData[ELEMENT_CHECKBOX] = GetRegInt("IncludeElementData");
	cntlDlgData.CheckBoxData[SEO_CHECKBOX] = GetRegInt("IncludeSEOData");
	cntlDlgData.CheckBoxData[NAVIGATOR_CHECKBOX] = GetRegInt("IncludeNavigatorData");
	cntlDlgData.CheckBoxData[LAYER_CHECKBOX] = GetRegInt("IncludeLayerData");
	cntlDlgData.CheckBoxData[PROFILE_CHECKBOX] = GetRegInt("IncludeProfileData");
	cntlDlgData.CheckBoxData[ZERO_CHECKBOX] = GetRegInt(GS::UniString("IncludeZeroValuedData"));
}



void ProcessSEO(CntlDlgData& io_cntlDlgData)
{
	AddItem("SEO Data", "Number of SEO Operators/Targets", GetSEOElements().GetSize(), io_cntlDlgData);
	AddItem("SEO Data", "Number of erroneous SEO Operators/Targets", GetSEOElements(true).GetSize(), io_cntlDlgData);
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

		//SetRegInt(33, 
		//	GS::UniString("tesztKey2"));

		DGSetItemValLong(dialID, ZERO_CHECKBOX, cntlDlgData.iAddZeroValues);

		//if (cntlDlgData.CheckBoxData[LIBPART_CHECKBOX]) ProcessLibParts(cntlDlgData, apiTypeDict, iLibPartInstanceS);
		//if (cntlDlgData.CheckBoxData[ELEMENT_CHECKBOX]) ProcessElements(cntlDlgData);
		//if (cntlDlgData.CheckBoxData[SEO_CHECKBOX]) ProcessSEO(cntlDlgData);
		//if (cntlDlgData.CheckBoxData[NAVIGATOR_CHECKBOX]) ProcessNavigatorItems(cntlDlgData);
		//if (cntlDlgData.CheckBoxData[LIBPART_CHECKBOX]) ProcessAttributes(cntlDlgData);
		if (cntlDlgData.CheckBoxData[PROFILE_CHECKBOX]) ProcessProfiles();

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

		//DGSetItemValLong(dialID, ZERO_CHECKBOX, cntlDlgData.iAddZeroValues);

		for (UInt16 i = LIBPART_CHECKBOX; i <= ZERO_CHECKBOX; i++)
			DGSetItemValLong(dialID, i, cntlDlgData.CheckBoxData[i]);
		break;
	}
	case DG_MSG_CLICK:
		switch (item) {
		case OK_BUTTON:
			//SetRegInt(cntlDlgData.CheckBoxData[LIBPART_CHECKBOX], GS::UniString("LibraryPartData"));
			//SetRegInt(cntlDlgData.CheckBoxData[ELEMENT_CHECKBOX], GS::UniString("IncludeElementData"));
			//SetRegInt(cntlDlgData.CheckBoxData[SEO_CHECKBOX], GS::UniString("IncludeSEOData"));
			//SetRegInt(cntlDlgData.CheckBoxData[NAVIGATOR_CHECKBOX], GS::UniString("IncludeNavigatorData"));
			//SetRegInt(cntlDlgData.CheckBoxData[LAYER_CHECKBOX], GS::UniString("IncludeLayerData"));
			//SetRegInt(cntlDlgData.CheckBoxData[PROFILE_CHECKBOX], GS::UniString("IncludeProfileData"));
			//SetRegInt(cntlDlgData.CheckBoxData[ZERO_CHECKBOX], GS::UniString("IncludeZeroValuedData"));

			result = item;
			break;
		case IMPORT_BUTTON:
			Do_ImportNamesFromExcel(cntlDlgData);

			result = item;
			break;
		}
	case DG_MSG_CHANGE:
		switch (item) {
		case LIBPART_CHECKBOX:
		case ELEMENT_CHECKBOX:
		case SEO_CHECKBOX:
		case NAVIGATOR_CHECKBOX:
		case LAYER_CHECKBOX:
		case PROFILE_CHECKBOX:
		case ZERO_CHECKBOX:
			//TODO Refresh the dialog
			cntlDlgData.iAddZeroValues = DGGetItemValLong(dialID, ZERO_CHECKBOX);
			for (UInt16 i = LIBPART_CHECKBOX; i <= ZERO_CHECKBOX; i++)
				cntlDlgData.CheckBoxData[i] = DGGetItemValLong(dialID, i);
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
	InitUI();

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

