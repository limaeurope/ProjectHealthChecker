#ifndef _SETTINGSSINGLETON_HPP
#define _SETTINGSSINGLETON_HPP

#include	<mutex>
#include	"APITypeDict.hpp"
#include	"Data/DataStructs.hpp"
#include	"Table/ResultTable.hpp"
#include	"Enums/CheckboxEnum.hpp"
#include	"Enums/StringsEnum.hpp"
#include	"Enums/Loglevels.hpp"


const double COLUMN_WIDTH = 32.0;

class SettingsSingleton {
	SettingsSingleton();
	~SettingsSingleton();
	SettingsSingleton(const SettingsSingleton&);
	void operator=(const SettingsSingleton&) {};

	Loglevel		m_loglevel;
	GS::UniString	m_appName;
	GS::UniString	m_companyName;
	GS::UniString	m_sLogFolder;
	GS::UniString	m_inExcel;
	GS::UniString	m_sOutExcel;
public:
	static const APITypeDict ApiTypeDict;
	GS::HashTable<int, Int32> CheckBoxData;
	GS::HashSet<GS::UniString> FilterStrings;
	GS::HashSet<GS::UniString> CompositeStrings;

	ResultTable	resultTable;
	AttributeUsage attributeUsage;

	inline static API_LibraryTypeID GetLibPartType(const API_LibPart& i_libPart) { return ApiTypeDict.GetLibPartType(i_libPart); };
	static SettingsSingleton& GetInstance();
	ResultSheet& GetSheet(const GS::UniString& i_sName);
	ResultSheet& GetSheet(const IntStr i_sName);
	bool ImportNamesFromExcel(const GS::UniString& i_sSheet = "");
	void SetLoglevel(const Loglevel i_loglevel);
	Loglevel GetLoglevel() const;
	inline GS::UniString GetAppName() const { return m_appName; } ;
	inline GS::UniString GetCompanyName() const { return m_companyName; } ;
	inline GS::UniString GetLogFolder() const { return m_sLogFolder; } ;
	inline void SetLogFolder(const GS::UniString& i_sLogFolder) { m_sLogFolder = i_sLogFolder; };
	inline GS::UniString GetImport() const { return m_inExcel; } ;
	inline void SetImport(const GS::UniString& i_sInExcel) { m_inExcel = i_sInExcel; };
	inline GS::UniString GetExport() const { return m_sOutExcel; };
	inline void SetExport(const GS::UniString& i_sOutExcel) { m_sOutExcel = i_sOutExcel; };
	void SetExcelFile();
};

static std::mutex _mutex;
extern SettingsSingleton& (*SETTINGS)();

#endif // !_SETTINGSSINGLETON_HPP

