#ifndef _SETTINGSSINGLETON_HPP
#define _SETTINGSSINGLETON_HPP

#include	<mutex>
#include	"APITypeDict.hpp"
#include	"Data/DataStructs.hpp"
#include	"Table/ResultTable.hpp"
#include	"Enums/CheckboxEnum.hpp"


#define LIBPART_CHECKBOX	2
#define ELEMENT_CHECKBOX	3
#define SEO_CHECKBOX		4
#define NAVIGATOR_CHECKBOX	5
#define LAYER_CHECKBOX		6
#define PROFILE_CHECKBOX	7
#define ZERO_CHECKBOX		8
#define COUNT_INSTANCES		9
#define CHECKBOX_MAX		COUNT_INSTANCES		
#define IMPORT_BUTTON		CHECKBOX_MAX + 1

const double COLUMN_WIDTH = 32.0;

class SettingsSingleton {
	SettingsSingleton();
	~SettingsSingleton();
	SettingsSingleton(const SettingsSingleton&);
	void operator=(const SettingsSingleton&) {};
public:
	static const APITypeDict ApiTypeDict;
	GS::HashTable<int, Int32> CheckBoxData;
	GS::HashSet<GS::UniString> FilterStrings;

	ResultTable	resultTable;
	AttributeUsage AttributeUsage;
	int iTest;

	inline static API_LibraryTypeID GetLibPartType(const API_LibPart& i_libPart) { return ApiTypeDict.GetLibPartType(i_libPart); };
	static SettingsSingleton& GetInstance();
	ResultSheet& GetSheet(const GS::UniString& i_sName);
};

static std::mutex _mutex;
extern SettingsSingleton& (*SETTINGS)();

#endif // !_SETTINGSSINGLETON_HPP

