#include	"SettingsSingleton.hpp"
#include	"Table/ResultTable.hpp"
#include	"ProjectHealthChecker.hpp"
#include	"WinReg.hpp"
#include	"Enums/CheckboxEnum.hpp"

#include	"Table/LibXLExtended.hpp"
#include	"Utils/Utils.hpp"
#include	"Utils/DateTime.hpp"

const APITypeDict SettingsSingleton::ApiTypeDict = APITypeDict();
SettingsSingleton& (*SETTINGS)() = SettingsSingleton::GetInstance;

GS::HashSet<GS::UniString> SetFromSheet(const GS::UniString& i_sSheet, const BookExtended* const i_book)
{
	GS::HashSet<GS::UniString> result;

	if (SheetExtended* sheet = i_book->_getSheet(i_sSheet))
	{
		for (int row = sheet->firstRow(); row < sheet->lastRow(); ++row)
		{
			if (const wchar_t* sFilter = sheet->readStr(row, 0))
				result.Add(GS::UniString(sFilter));
		}
	}

	return result;
}

Loglevel SettingsSingleton::GetLoglevel() const 
{ 
	return m_loglevel; 
}

void SettingsSingleton::SetLoglevel(const Loglevel i_loglevel) 
{ 
	m_loglevel = i_loglevel; 
}

SettingsSingleton::SettingsSingleton()
{
	//Can't use LOGGER in constructor due to dependency TODO

	m_companyName = GetStringFromResource_(32506, 1);
	m_appName = GetStringFromResource_(32000, 1);

	CheckBoxData.Put(Libpart_checkbox,		GetRegInt("LibraryPartData"));
	CheckBoxData.Put(Element_checkbox,		GetRegInt("IncludeElementData"));
	CheckBoxData.Put(SEO_checkbox,			GetRegInt("IncludeSEOData"));
	CheckBoxData.Put(Navigator_checkbox,	GetRegInt("IncludeNavigatorData"));
	CheckBoxData.Put(Layer_checkbox,		GetRegInt("IncludeLayerData"));
	CheckBoxData.Put(Profile_checkbox,		GetRegInt("IncludeProfileData"));
	CheckBoxData.Put(Count_instances,		GetRegInt("CountInstances"));
	CheckBoxData.Put(Zero_checkbox,			GetRegInt(GS::UniString("IncludeZeroValuedData")));
	CheckBoxData.Put(Consistency_checkbox,	GetRegInt("IncludeConsistency"));
	CheckBoxData.Put(AutoImport,				GetRegInt("AutoImport"));
	CheckBoxData.Put(AutoExport,				GetRegInt("AutoExport"));

	SetLoglevel((Loglevel)GetRegInt("Loglevel"));
	SetLogFolder(GetRegString("LogFileFolder"));
	SetImport(GetRegString("ImportExcel"));
	SetExport(GetRegString("OutputExcel"));

	if (CheckBoxData[AutoImport] && GetImport() != "")
		ImportNamesFromExcel();
}

SettingsSingleton::~SettingsSingleton()
{
	SetRegInt(CheckBoxData[Libpart_checkbox],		"LibraryPartData");
	SetRegInt(CheckBoxData[Element_checkbox],		"IncludeElementData");
	SetRegInt(CheckBoxData[SEO_checkbox],			"IncludeSEOData");
	SetRegInt(CheckBoxData[Navigator_checkbox],		"IncludeNavigatorData");
	SetRegInt(CheckBoxData[Layer_checkbox],			"IncludeLayerData");
	SetRegInt(CheckBoxData[Profile_checkbox],		"IncludeProfileData");
	SetRegInt(CheckBoxData[Count_instances],		"CountInstances");
	SetRegInt(CheckBoxData[Zero_checkbox], GS::UniString("IncludeZeroValuedData"));
	SetRegInt(CheckBoxData[Consistency_checkbox],	"IncludeConsistency");
	SetRegInt(CheckBoxData[AutoImport],				"AutoImport");
	SetRegInt(CheckBoxData[AutoExport],				"AutoExport");

	SetRegString(GetLogFolder(), "LogFileFolder");
	SetRegInt((short)GetLoglevel(), "Loglevel");


	if (SETTINGS().CheckBoxData[AutoImport])
		SetRegString(GetImport(), "ImportExcel");
	else
		SetRegString("", "ImportExcel");


	if (SETTINGS().CheckBoxData[AutoExport])
		SetRegString(GetExport(), "OutputExcel");
	else
		SetRegString("", "OutputExcel");
}

SettingsSingleton& SettingsSingleton::GetInstance()
{
	std::lock_guard<std::mutex> lock(_mutex);

	static SettingsSingleton singleton;

	return singleton;
}

ResultSheet& SettingsSingleton::GetSheet(const GS::UniString& i_sName)
{
	if (!resultTable.sheetDict.ContainsKey(i_sName))
		resultTable.sheetDict.Add(i_sName, ResultSheet{});

	return resultTable.sheetDict[i_sName];
}

ResultSheet& SettingsSingleton::GetSheet(const IntStr i_sName)
{
	return GetSheet(GSFR(i_sName));
}

bool SettingsSingleton::ImportNamesFromExcel(const GS::UniString& i_sSheet /*= ""*/)
{
	GS::UniString filepath;

	if	(	(CheckBoxData[AutoImport])
		&&  (GetImport() != ""))
	{
		filepath = GetImport();
	}
	else
	{
		return false;
	}

	BookExtended* book;
	bool isBookLoaded = false;

	book = (BookExtended*)xlCreateBook();
	if (book->load(filepath.ToUStr()))
		isBookLoaded = true;
	else
	{
		book = (BookExtended*)xlCreateXMLBook();
		if (book->load(filepath.ToUStr()))
			isBookLoaded = true;
	}

	if (isBookLoaded)
	{
		FilterStrings = SetFromSheet("CountLayers", book);
		CompositeStrings = SetFromSheet("Composite Consistency", book);

		book->release();
		return true;
	}
	else
		return false;
}

void SettingsSingleton::SetExcelFile()
{
	IO::Location xlsFileLoc;
	if (!GetOpenFile(&xlsFileLoc, "xlsx", "*.xlsx", DG::FileDialog::OpenFile))
		return;

	xlsFileLoc.ToPath(&m_inExcel);
}

