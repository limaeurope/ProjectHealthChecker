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


void ProcessElements(CntlDlgData& io_cntlDlgData)
{
	for (UINT16 i = 1; i <= ac_types.GetSize(); i++)
	{
		GS::Array<API_Guid> _array{};
		char intStr[256], _sNumberOfWalls[256], _sNumberOfWalls2[256];

		GSErrCode err = ACAPI_Element_GetElemList(static_cast<API_ElemTypeID>(i), &_array);

		auto _a = ac_types[i - 1].ToCStr().Get();
		sprintf(_sNumberOfWalls2, "Number of %s", _a);
		AddItem("Elements", _sNumberOfWalls2, _array.GetSize(), io_cntlDlgData);
	}
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

		DGSetItemValLong(dialID, ZERO_CHECKBOX, cntlDlgData.iAddZeroValues);

		ProcessLibPars(cntlDlgData, apiTypeDict, iLibPartInstanceS);
		ProcessElements(cntlDlgData);
		ProcessSEO(cntlDlgData);
		ProcessNavigatorItems(cntlDlgData);
		ProcessAttributes(cntlDlgData);

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

