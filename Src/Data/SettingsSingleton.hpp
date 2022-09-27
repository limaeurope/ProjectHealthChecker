#ifndef _SETTINGSSINGLETON_HPP
#define _SETTINGSSINGLETON_HPP

#include	<mutex>
#include	"APITypeDict.hpp"
#include	"Data/DataStructs.hpp"
#include	"Table/ResultTable.hpp"


#define LIBPART_CHECKBOX	2
#define ELEMENT_CHECKBOX	3
#define SEO_CHECKBOX		4
#define NAVIGATOR_CHECKBOX	5
#define LAYER_CHECKBOX		6
#define PROFILE_CHECKBOX	7
#define ZERO_CHECKBOX		8
#define COUNT_INSTANCES		9
#define MAX_CHECKBOX		COUNT_INSTANCES		
#define IMPORT_BUTTON		10

class SettingsSingleton {
	SettingsSingleton();
	~SettingsSingleton();
	SettingsSingleton(const SettingsSingleton&);
	bool operator=(const SettingsSingleton&) {};
public:
	static const APITypeDict ApiTypeDict;
	GS::HashTable<int, Int32> CheckBoxData;
	GS::HashTable<GS::UniString, ReportData> ReportData;
	//GS::HashTable<GS::UniString, ReportDataHeader> ReportHeaders;
	GS::HashSet<GS::UniString> FilterStrings;

	ResultTable	ResultTable;

	inline static API_LibraryTypeID GetLibPartType(const API_LibPart& i_libPart) { return ApiTypeDict.GetLibPartType(i_libPart); };
	static SettingsSingleton& GetInstance();
};

static std::mutex _mutex;

#endif // !_SETTINGSSINGLETON_HPP

