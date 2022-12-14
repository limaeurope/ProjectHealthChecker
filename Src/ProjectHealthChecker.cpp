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
#include	"AttributeUsage.hpp"
#include	"../Constants/loglevelStrings.hpp"
#include	"Utils/Logger.hpp"

// ---------------------------------- Types ------------------------------------

// ---------------------------------- Variables --------------------------------

DGMessageData								cntlDlgData;	//Dummy, unused

// ----------------------------------  -------------------------------

void ProcessSEO()
{
	SETTINGS().GetSheet("SEO Data").AddItem("Number of SEO Operators/Targets", GetSEOElements().GetSize());
	SETTINGS().GetSheet("SEO Data").AddItem("Number of erroneous SEO Operators/Targets", GetSEOElements(true).GetSize());
}



static short DGCALLBACK CntlDlgCallBack(short message, short dialID, short item, DGUserData userData, DGMessageData msgData)
{
	short result = 0;
	GS::UniString _text{};
	API_PropertyDefinition _def;

	switch (message) {
	case DG_MSG_INIT:
	{
		SETTINGS().attributeUsage.ProcessAttributeUsage();

		if (SETTINGS().CheckBoxData[Libpart_checkbox]) ProcessLibParts();
		if (SETTINGS().CheckBoxData[Element_checkbox]) ProcessElements();
		if (SETTINGS().CheckBoxData[SEO_checkbox]) ProcessSEO();
		if (SETTINGS().CheckBoxData[Navigator_checkbox]) ProcessNavigatorItems();
		if (SETTINGS().CheckBoxData[Layer_checkbox]) ProcessAttributes();
		if (SETTINGS().CheckBoxData[Profile_checkbox]) ProcessProfiles();
		if (SETTINGS().CheckBoxData[Consistency_checkbox]) CheckConsistency();

		break;
	}
	case DG_MSG_CLICK:
		switch (item) {
		case OK_BUTTON:
			if (SETTINGS().CheckBoxData[AutoExport])
				SETTINGS().resultTable.ExportReportToExcel();

			result = item;
			break;
		case EXPORT_BUTTON:
			SETTINGS().resultTable.ExportReportToExcel();

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
	IO::Location sFileLoc;

	switch (message) {
	case DG_MSG_INIT:
	{
		GSErrCode err;

		for (UInt16 i = Libpart_checkbox; i <= Checkbox_max; i++)
			DGSetItemValLong(dialID, i, SETTINGS().CheckBoxData[i]);

		for (auto _s: sLoglevels)
		{
			DGPopUpInsertItem(dialID, Loglevel_Popup, DG_LIST_BOTTOM);
			DGPopUpSetItemText(dialID, Loglevel_Popup, DG_LIST_BOTTOM, _s);
		}

		DGPopUpSelectItem(dialID, Loglevel_Popup, SETTINGS().GetLoglevel() + 1);

		DGSetItemText(dialID, Import_Text, SETTINGS().GetImport());
		DGSetItemText(dialID, Export_Text, SETTINGS().GetExport());

		break;
	}
	case DG_MSG_CLICK:
		switch (item) {
		case OK_BUTTON:
		
			break;
		case Import_button:
			SETTINGS().SetExcelFile();
			SETTINGS().ImportNamesFromExcel();

			break;
		case Export_button:
			if (GetOpenFile(&sFileLoc, "xlsx", "*.xlsx", DG::FileDialog::Save))
			{
				GS::UniString _path;
				GSErrCode err = sFileLoc.ToPath(&_path);
				SETTINGS().SetExport(_path);
			}

			break;
		case LogFolder_Button:
			if (GetOpenFile(&sFileLoc, DG::FileDialog::OpenFolder))
			{
				GS::UniString _path;
				GSErrCode err = sFileLoc.ToPath(&_path);
				SETTINGS().SetLogFolder(_path);
			}

			break;
		}

		result = item;

		break;
	case DG_MSG_CHANGE:
		switch (item) {
		case Libpart_checkbox:
		case Element_checkbox:
		case SEO_checkbox:
		case Navigator_checkbox:
		case Layer_checkbox:
		case Profile_checkbox:
		case Zero_checkbox:
		case Count_instances:
		case Consistency_checkbox:
			for (UInt16 i = Libpart_checkbox; i <= Checkbox_max; i++)
				SETTINGS().CheckBoxData[i] = DGGetItemValLong(dialID, i);

			if (!SETTINGS().CheckBoxData[AutoExport])
				SETTINGS().SetExport("");

			break;
		case AutoImport:
			if (!DGGetItemValLong(dialID, AutoImport))
			{
				SETTINGS().CheckBoxData[AutoImport] = 0;
				SETTINGS().SetImport("");
			}
			else
				if (GetOpenFile(&sFileLoc, "xlsx", "*.xlsx", DG::FileDialog::OpenFile))
				{
					GS::UniString _path;
					GSErrCode err = sFileLoc.ToPath(&_path);
					SETTINGS().CheckBoxData[AutoImport] = 1;
					SETTINGS().SetImport(_path);

					if (!SETTINGS().ImportNamesFromExcel())
					{
						SETTINGS().CheckBoxData[AutoImport] = 0;
						SETTINGS().SetImport("");
					}
				}
				else
				{
					SETTINGS().CheckBoxData[AutoImport] = 0;
					SETTINGS().SetImport("");
				}

				DGSetItemValLong(dialID, AutoImport, SETTINGS().CheckBoxData[AutoImport]);
				DGSetItemText(dialID, Import_Text, SETTINGS().GetImport());
			break;
		case AutoExport:
			if (!DGGetItemValLong(dialID, AutoExport))
			{
				SETTINGS().CheckBoxData[AutoExport] = 0;
				SETTINGS().SetExport("");
			}
			else
				if (GetOpenFile(&sFileLoc, "xlsx", "*.xlsx", DG::FileDialog::Save))
				{
					GS::UniString _path;
					GSErrCode err = sFileLoc.ToPath(&_path);
					SETTINGS().CheckBoxData[AutoExport] = 1;
					SETTINGS().SetExport(_path);
				}
				else
				{
					SETTINGS().CheckBoxData[AutoExport] = 0;
					SETTINGS().SetExport("");
				}
			
				DGSetItemValLong(dialID, AutoExport, SETTINGS().CheckBoxData[AutoExport]);
				DGSetItemText(dialID, Export_Text, SETTINGS().GetExport());
			break;
		case Export_Text:
			//FIXME checks
			SETTINGS().SetExport(DGGetItemText(dialID, Export_Text));

			break;
		case Import_Text:
			//FIXME checks
			SETTINGS().SetImport(DGGetItemText(dialID, Import_Text));

			break;
		case Loglevel_Popup:
			SETTINGS().SetLoglevel((Loglevel)(DGPopUpGetSelected(dialID, Loglevel_Popup) - 1));
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

