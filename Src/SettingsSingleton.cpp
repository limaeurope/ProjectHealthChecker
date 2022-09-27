#include	"SettingsSingleton.hpp"
#include	"Table/ResultTable.hpp"
#include	"ProjectHealthChecker.hpp"
#include	"WinReg.hpp"

const APITypeDict SettingsSingleton::ApiTypeDict = APITypeDict();

SettingsSingleton::SettingsSingleton()
{
	CheckBoxData = {0, 0, 1, 1, 1, 1, 1, 1, 1};
	ReportData = {};
	ReportHeaders = {};
	FilterStrings = {};


	CheckBoxData[LIBPART_CHECKBOX]		= GetRegInt("LibraryPartData");
	CheckBoxData[ELEMENT_CHECKBOX]		= GetRegInt("IncludeElementData");
	CheckBoxData[SEO_CHECKBOX]			= GetRegInt("IncludeSEOData");
	CheckBoxData[NAVIGATOR_CHECKBOX]	= GetRegInt("IncludeNavigatorData");
	CheckBoxData[LAYER_CHECKBOX]		= GetRegInt("IncludeLayerData");
	CheckBoxData[PROFILE_CHECKBOX]		= GetRegInt("IncludeProfileData");
	CheckBoxData[ZERO_CHECKBOX]	= GetRegInt(GS::UniString("IncludeZeroValuedData"));
}

SettingsSingleton::~SettingsSingleton()
{
	SetRegInt(CheckBoxData[LIBPART_CHECKBOX],	"LibraryPartData");
	SetRegInt(CheckBoxData[ELEMENT_CHECKBOX],	"IncludeElementData");
	SetRegInt(CheckBoxData[SEO_CHECKBOX],		"IncludeSEOData");
	SetRegInt(CheckBoxData[NAVIGATOR_CHECKBOX],	"IncludeNavigatorData");
	SetRegInt(CheckBoxData[LAYER_CHECKBOX],		"IncludeLayerData");
	SetRegInt(CheckBoxData[PROFILE_CHECKBOX],	"IncludeProfileData");
	SetRegInt(CheckBoxData[ZERO_CHECKBOX], GS::UniString("IncludeZeroValuedData"));
}

SettingsSingleton& SettingsSingleton::GetInstance()
{
	std::lock_guard<std::mutex> lock(_mutex);

	static SettingsSingleton singleton;

	return singleton;
}

