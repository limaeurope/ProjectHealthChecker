#include	"SettingsSingleton.hpp"
#include	"Table/ResultTable.hpp"
#include	"ProjectHealthChecker.hpp"
#include	"WinReg.hpp"


SettingsSingleton::SettingsSingleton()
{
	cntlDlgData.CheckBoxData = GS::Array<Int32>{0, 0, 1, 1, 1, 1, 1, 1, 1};
	//cntlDlgData.reportData  = GS::HashTable<GS::UniString, ReportData>{};
	//cntlDlgData.reportHeaders = GS::HashTable<GS::UniString, ReportDataHeader>{};
	//cntlDlgData.filterStrings = GS::HashSet<GS::UniString>{};

	cntlDlgData.CheckBoxData[LIBPART_CHECKBOX] = GetRegInt("LibraryPartData");
	cntlDlgData.CheckBoxData[ELEMENT_CHECKBOX] = GetRegInt("IncludeElementData");
	cntlDlgData.CheckBoxData[SEO_CHECKBOX] = GetRegInt("IncludeSEOData");
	cntlDlgData.CheckBoxData[NAVIGATOR_CHECKBOX] = GetRegInt("IncludeNavigatorData");
	cntlDlgData.CheckBoxData[LAYER_CHECKBOX] = GetRegInt("IncludeLayerData");
	cntlDlgData.CheckBoxData[PROFILE_CHECKBOX] = GetRegInt("IncludeProfileData");
	cntlDlgData.CheckBoxData[ZERO_CHECKBOX] = GetRegInt(GS::UniString("IncludeZeroValuedData"));
}

SettingsSingleton::~SettingsSingleton()
{
	SetRegInt(cntlDlgData.CheckBoxData[LIBPART_CHECKBOX], GS::UniString("LibraryPartData"));
	SetRegInt(cntlDlgData.CheckBoxData[ELEMENT_CHECKBOX], GS::UniString("IncludeElementData"));
	SetRegInt(cntlDlgData.CheckBoxData[SEO_CHECKBOX], GS::UniString("IncludeSEOData"));
	SetRegInt(cntlDlgData.CheckBoxData[NAVIGATOR_CHECKBOX], GS::UniString("IncludeNavigatorData"));
	SetRegInt(cntlDlgData.CheckBoxData[LAYER_CHECKBOX], GS::UniString("IncludeLayerData"));
	SetRegInt(cntlDlgData.CheckBoxData[PROFILE_CHECKBOX], GS::UniString("IncludeProfileData"));
	SetRegInt(cntlDlgData.CheckBoxData[ZERO_CHECKBOX], GS::UniString("IncludeZeroValuedData"));

	delete singleton;
}

SettingsSingleton* SettingsSingleton::GetInstance()
{
	std::lock_guard<std::mutex> lock(_mutex);

	if (!singleton)
		singleton = new SettingsSingleton();

	return singleton;
}

