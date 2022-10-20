#include	"SettingsSingleton.hpp"
#include	"Table/ResultTable.hpp"
#include	"ProjectHealthChecker.hpp"
#include	"WinReg.hpp"
#include	"Enums/CheckboxEnum.hpp"

#include	"Excel.hpp"
#include	"Table/LibXLExtended.hpp"
#include	"Utils/Utils.hpp"

#include	<ctime>

const APITypeDict SettingsSingleton::ApiTypeDict = APITypeDict();
SettingsSingleton& (*SETTINGS)() = SettingsSingleton::GetInstance;

SettingsSingleton::SettingsSingleton()
{
	m_companyName = GetStringFromResource_(32506, 1);
	m_appName = GetStringFromResource_(32000, 1);

	FilterStrings = {};
	
	CheckBoxData.Put(Libpart_checkbox,	GetRegInt("LibraryPartData"));
	CheckBoxData.Put(Element_checkbox,	GetRegInt("IncludeElementData"));
	CheckBoxData.Put(SEO_checkbox,		GetRegInt("IncludeSEOData"));
	CheckBoxData.Put(Navigator_checkbox,GetRegInt("IncludeNavigatorData"));
	CheckBoxData.Put(Layer_checkbox,	GetRegInt("IncludeLayerData"));
	CheckBoxData.Put(Profile_checkbox,	GetRegInt("IncludeProfileData"));
	CheckBoxData.Put(Count_instances,	GetRegInt("CountInstances"));
	CheckBoxData.Put(Zero_checkbox,		GetRegInt(GS::UniString("IncludeZeroValuedData")));

	GS::UniString logFileFolder = GetRegString("LogFileFolder");

	if (logFileFolder.GetLength() > 0)
	{
		auto _m = logFileFolder.ToCStr();

		m_logger.SetLogFileFolder(logFileFolder);
	}
	else
	{
		time_t now = time(0);
		tm* ltm = localtime(&now);

		char buffer[256];

		strftime(buffer, sizeof(buffer), "%a %b %d %H:%M:%S %Y", ltm);

		IO::Location loc;

		//GSErrCode err = IO::fileSystem.GetSpecialLocation(IO::FileSystem::TemporaryFolder, &loc);
		GSErrCode err = IO::fileSystem.GetSpecialLocation(IO::FileSystem::Desktop, &loc);
	
		loc.AppendToLocal(IO::Name(m_companyName));
		loc.AppendToLocal(IO::Name(m_appName));

		loc.AppendToLocal(IO::Name(GS::UniString("ProjectHealtChecker") + GS::UniString(buffer) + GS::UniString(".log")));

		m_logger.SetLogFileFolder(loc);
	}
}

SettingsSingleton::~SettingsSingleton()
{
	SetRegInt(CheckBoxData[Libpart_checkbox],	"LibraryPartData");
	SetRegInt(CheckBoxData[Element_checkbox],	"IncludeElementData");
	SetRegInt(CheckBoxData[SEO_checkbox],		"IncludeSEOData");
	SetRegInt(CheckBoxData[Navigator_checkbox],	"IncludeNavigatorData");
	SetRegInt(CheckBoxData[Layer_checkbox],		"IncludeLayerData");
	SetRegInt(CheckBoxData[Profile_checkbox],	"IncludeProfileData");
	SetRegInt(CheckBoxData[Count_instances],	"CountInstances");
	SetRegInt(CheckBoxData[Zero_checkbox], GS::UniString("IncludeZeroValuedData"));

	SetRegString(m_logger.GetLogFileFolderStr(), "LogFileFolder");
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

void SettingsSingleton::ImportNamesFromExcel(const GS::UniString& i_sSheet /*= ""*/)
{
	IO::Location xlsFileLoc;
	if (!GetOpenFile(&xlsFileLoc, "xlsx", "*.xlsx", DG::FileDialog::OpenFile))
		return;

	GS::UniString filepath;
	xlsFileLoc.ToPath(&filepath);

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
		if (SheetExtended* sheet = book->_getSheet("CountLayers"))
		{
			for (int row = sheet->firstRow(); row < sheet->lastRow(); ++row)
			{
				if (const wchar_t* sFilter = sheet->readStr(row, 0))
					FilterStrings.Add(GS::UniString(sFilter));
			}
		}

		//if (SheetExtended* sheet = book->_getSheet("CountLayers"))
		//{
		//	for (int row = sheet->firstRow(); row < sheet->lastRow(); ++row)
		//	{
		//		if (const wchar_t* sFilter = sheet->readStr(row, 0))
		//			ListStrings.Add(GS::UniString(sFilter));
		//	}
		//}

		book->release();
	}
}

