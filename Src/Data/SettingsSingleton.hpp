#ifndef _SETTINGSSINGLETON_HPP
#define _SETTINGSSINGLETON_HPP

#include	<mutex>
#include	"APITypeDict.hpp"
#include	"Data/DataStructs.hpp"
#include	"Table/ResultTable.hpp"
#include	"Enums/CheckboxEnum.hpp"
#include	"Enums/StringsEnum.hpp"


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
	//GS::HashSet<GS::UniString> ListStrings;

	ResultTable	resultTable;
	AttributeUsage AttributeUsage;
	int iTest;

	inline static API_LibraryTypeID GetLibPartType(const API_LibPart& i_libPart) { return ApiTypeDict.GetLibPartType(i_libPart); };
	static SettingsSingleton& GetInstance();
	ResultSheet& GetSheet(const GS::UniString& i_sName);
	ResultSheet& GetSheet(const IntStr i_sName);
	void ImportNamesFromExcel(const GS::UniString& i_sSheet = "");
};

static std::mutex _mutex;
extern SettingsSingleton& (*SETTINGS)();

#endif // !_SETTINGSSINGLETON_HPP

