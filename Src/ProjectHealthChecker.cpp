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
#include	"Data/SettingsSingleton.hpp"
#include	"Data/WinReg.hpp"
#include	"ACSpecific/Element.hpp"
#include	"ACSpecific/SEO.hpp"
#include	"ACSpecific/LibPart.hpp"
#include	"ACSpecific/Attribute.hpp"
#include	"ACSpecific/Navigator.hpp"
#include	"ACSpecific/Profile.hpp"
#include	"Table/Excel.hpp"
#include	"AttributeUsage.hpp"


// ---------------------------------- Types ------------------------------------

// ---------------------------------- Variables --------------------------------

DGMessageData								cntlDlgData;	//Dummy
static GS::HashTable<GS::UniString,	UInt32> iLibPartInstanceS{};
static ResultTable							resultTable;

//static AttributeUsage						attributeUsage;
// ----------------------------------  -------------------------------

void ProcessSEO()
{
	AddItem("SEO Data", "Number of SEO Operators/Targets", GetSEOElements().GetSize());
	AddItem("SEO Data", "Number of erroneous SEO Operators/Targets", GetSEOElements(true).GetSize());
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

		AttributeUsage attributeUsage{};

		if (SETTINGS().CheckBoxData[LIBPART_CHECKBOX]) ProcessLibParts(iLibPartInstanceS);
		if (SETTINGS().CheckBoxData[ELEMENT_CHECKBOX]) ProcessElements();
		if (SETTINGS().CheckBoxData[SEO_CHECKBOX]) ProcessSEO();
		if (SETTINGS().CheckBoxData[NAVIGATOR_CHECKBOX]) ProcessNavigatorItems();
		if (SETTINGS().CheckBoxData[LAYER_CHECKBOX]) ProcessAttributes(attributeUsage);
		if (SETTINGS().CheckBoxData[PROFILE_CHECKBOX]) ProcessProfiles();

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

		for (UInt16 i = LIBPART_CHECKBOX; i <= CHECKBOX_MAX; i++)
			DGSetItemValLong(dialID, i, SETTINGS().CheckBoxData[i]);
		break;
	}
	case DG_MSG_CLICK:
		switch (item) {
		case OK_BUTTON:
			result = item;
			break;
		case IMPORT_BUTTON:
			Do_ImportNamesFromExcel();

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
		case COUNT_INSTANCES:
			//TODO Refresh the dialog
			for (UInt16 i = LIBPART_CHECKBOX; i <= CHECKBOX_MAX; i++)
				SETTINGS().CheckBoxData[i] = DGGetItemValLong(dialID, i);
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

