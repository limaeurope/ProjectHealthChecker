#include	"SettingsSingleton.hpp"
#include	"Table/ResultTable.hpp"
#include	"ProjectHealthChecker.hpp"
#include	"WinReg.hpp"

const APITypeDict SettingsSingleton::ApiTypeDict = APITypeDict();
SettingsSingleton& (*SETTINGS)() = SettingsSingleton::GetInstance;

SettingsSingleton::SettingsSingleton()
{
	ReportData = {};
	FilterStrings = {};

	CheckBoxData.Put(LIBPART_CHECKBOX,	GetRegInt("LibraryPartData"));
	CheckBoxData.Put(ELEMENT_CHECKBOX,	GetRegInt("IncludeElementData"));
	CheckBoxData.Put(SEO_CHECKBOX,		GetRegInt("IncludeSEOData"));
	CheckBoxData.Put(NAVIGATOR_CHECKBOX,GetRegInt("IncludeNavigatorData"));
	CheckBoxData.Put(LAYER_CHECKBOX,	GetRegInt("IncludeLayerData"));
	CheckBoxData.Put(PROFILE_CHECKBOX,	GetRegInt("IncludeProfileData"));
	CheckBoxData.Put(COUNT_INSTANCES,	GetRegInt("CountInstances"));
	CheckBoxData.Put(ZERO_CHECKBOX,		GetRegInt(GS::UniString("IncludeZeroValuedData")));
}

SettingsSingleton::~SettingsSingleton()
{
	SetRegInt(CheckBoxData[LIBPART_CHECKBOX],	"LibraryPartData");
	SetRegInt(CheckBoxData[ELEMENT_CHECKBOX],	"IncludeElementData");
	SetRegInt(CheckBoxData[SEO_CHECKBOX],		"IncludeSEOData");
	SetRegInt(CheckBoxData[NAVIGATOR_CHECKBOX],	"IncludeNavigatorData");
	SetRegInt(CheckBoxData[LAYER_CHECKBOX],		"IncludeLayerData");
	SetRegInt(CheckBoxData[PROFILE_CHECKBOX],	"IncludeProfileData");
	SetRegInt(CheckBoxData[COUNT_INSTANCES],	"CountInstances");
	SetRegInt(CheckBoxData[ZERO_CHECKBOX], GS::UniString("IncludeZeroValuedData"));
}

SettingsSingleton& SettingsSingleton::GetInstance()
{
	std::lock_guard<std::mutex> lock(_mutex);

	static SettingsSingleton singleton;

	return singleton;
}

