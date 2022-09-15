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


// ---------------------------------- Types ------------------------------------


typedef GS::HashTable<GS::UniString, UInt32> ReportData;

struct CntlDlgData {
	Int32 iAddZeroValues;
	GS::HashTable<GS::UniString, ReportData> reportData;
	GS::HashSet<GS::UniString> filterStrings;
};

struct AbstractData {};

struct StringData :AbstractData {
	GS::UniString string;
	StringData(GS::UniString s) :string(s) {}
};

struct DataObject {
};

struct FileSizeReportObject : public DataObject
{
	GS::UniString path;
	GS::UniString name;
	UInt64 size;
	bool isEmbedded;
	API_LibraryTypeID libType;
	UInt32 nInstances = 0;
};

// ---------------------------------- Variables --------------------------------

static CntlDlgData			cntlDlgData{1};
static APITypeDict			apiTypeDict;
static GS::HashTable<GS::UniString, UInt32> iLibPartInstanceS{};

#define OK_BUTTON			1
#define SOURCE_GROUP_POPUP	2
#define EXPORT_BUTTON		3

#define ZERO_CHECKBOX		2
#define IMPORT_BUTTON		3

// ----------------------------------  -------------------------------

static bool hasTexture(API_Attribute i_apiAttrib, AbstractData* i_attrs)
{
	return i_apiAttrib.material.texture.fileLoc != NULL;
}

static bool nameContains(API_Attribute i_apiAttrib, AbstractData* i_attrs)
{
	return GS::UniString(i_apiAttrib.header.name)
		.FindFirst(static_cast<StringData*>(i_attrs)->string) < MaxUIndex;
}

static DataObject* getTextureSize(API_Attribute i_apiAttrib)
{
	if (i_apiAttrib.material.texture.status == 0) 
		throw 1;
	IO::Location loc{ *i_apiAttrib.material.texture.fileLoc };
	IO::File f{loc};
	GSErrCode err;
	GS::UniString path = "";
	UInt64 fileSize = 0;

	if (!loc.IsEmpty())
	{
		FileSizeReportObject* result = new FileSizeReportObject;
		
		err = loc.ToPath(&path);
		if (err) throw err;
		
		result->path = path;
		if (path.Contains("\\"))
			result->name = path(path.FindLast("\\") + 1, path.GetLength() - path.FindLast("\\") - 1);

		err = f.GetDataLength(&fileSize);
		if (err) throw err;

		result->size = fileSize;

		return result;
	}
	else throw 1;
}

// -----------------------------------------------------------------------------
//  List libparts
// -----------------------------------------------------------------------------

static GS::Array<DataObject*> ListLibParts()
{
	API_LibPart  libPart;
	Int32        i, count;
	GSErrCode    err;
	GS::UniString path = "";
	UInt64 fileSize = 0;
	GS::Array<DataObject*> aResult;
	GS::Array<API_LibraryInfo>    libInfo;
	IO::Path    sEmbeddedLibPath = "";

	if (ACAPI_Environment(APIEnv_GetLibrariesID, &libInfo) == NoError) {
		for (auto lib: libInfo) {
			if (lib.libraryType == API_EmbeddedLibrary)
			lib.location.ToPath(&sEmbeddedLibPath);
		}
	}

	err = ACAPI_LibPart_GetNum(&count);

	if (!err) {
		for (i = 1; i <= count; i++) {
			try
			{
				BNZeroMemory(&libPart, sizeof(API_LibPart));
				libPart.index = i;
				err = ACAPI_LibPart_Get(&libPart);
				if (!err) {
					IO::Location loc = *libPart.location;
					IO::File f{ loc };

					FileSizeReportObject* result = new FileSizeReportObject;

					err = loc.ToPath(&path);
					if (err) throw err;

					result->path = path;
					if (path.Contains("\\"))
						result->name = path(path.FindLast("\\") + 1, path.GetLength() - path.FindLast("\\") - 1);

					err = f.GetDataLength(&fileSize);
					if (err) throw err;

					result->size = fileSize;

					if (sEmbeddedLibPath)
					{
						result->isEmbedded = path.Contains(GS::UniString(sEmbeddedLibPath)) ? true : false;
					}

					result->libType = apiTypeDict.GetLibPartType(libPart);

					aResult.Push(result);

					if (libPart.location != nullptr)
						delete libPart.location;
				}
			}
			catch (...)
			{
				continue;
			}
		}
	}

	return aResult;
}

inline GS::UniString GetLibPartName(const API_LibPart& i_libPart)
{
	GS::UniString sResult;

	IO::Location loc = *i_libPart.location;
	IO::File f{ loc };

	GSErrCode err = loc.ToPath(&sResult);

	return sResult;
}

static void CountLibPartInstances()
{
	GS::Array<API_Guid> libPartGuidS;
	API_LibPart			libPart;
	API_Element         element;
	GSErrCode err;
	GS::UniString sLibPart;

	for (auto _id : GS::Array<API_ElemTypeID>{ 
		API_ObjectID, 
		API_LampID, 
		API_DoorID, 
		API_WindowID, 
		API_SkylightID })
	{
		err = ACAPI_Element_GetElemList(_id, &libPartGuidS);
	}

	Int32 iLibParts;
	err = ACAPI_LibPart_GetNum(&iLibParts);
	for (Int32 i = 1; i <= iLibParts; i++) {
		BNZeroMemory(&libPart, sizeof(API_LibPart));
		libPart.index = i;
		err = ACAPI_LibPart_Get(&libPart);
		if (!err && libPart.isPlaceable) {
			iLibPartInstanceS.Add(GetLibPartName(libPart), 0);
		}
		if (libPart.location != nullptr)
			delete libPart.location;
	}
	
	for (auto libPartGuid : libPartGuidS)
	{
		try
		{
			BNZeroMemory(&element, sizeof(API_Element));
			element.header.guid = libPartGuid;
			err = ACAPI_Element_Get(&element);
			if (err) throw err;

			BNZeroMemory(&libPart, sizeof(API_LibPart));
			if	(	element.header.typeID == API_ObjectID
				||	element.header.typeID == API_LampID)
			{
				libPart.index = element.object.libInd;
				err = ACAPI_LibPart_Get(&libPart);
			}
			else if	(	element.header.typeID == API_DoorID
					||	element.header.typeID == API_WindowID
					||	element.header.typeID == API_SkylightID)
			{
				err = ACAPI_Goodies(APIAny_GetElemLibPartUnIdID, &element.header, libPart.ownUnID);
				err = ACAPI_LibPart_Search(&libPart, false);
			}
			if (err) throw err;

			if (iLibPartInstanceS.ContainsKey(sLibPart = (GetLibPartName(libPart))))
			{
				iLibPartInstanceS[sLibPart]++;
			}
			else
			{
				iLibPartInstanceS.Add(sLibPart, 1);
			}
		}
		catch(...)
		{
			continue;
		}
	}
}

// -----------------------------------------------------------------------------
//  List attributes
// -----------------------------------------------------------------------------

static UInt32 CountAttributes(
	API_AttrTypeID i_attrType, 
	bool(*i_func)(API_Attribute, AbstractData*) = nullptr,
	AbstractData* i_attrs = nullptr)
{
	API_Attribute		attrib;
	API_AttributeIndex	count;
	GSErrCode			err;
	Int32 i, result = 0;

	err = ACAPI_Attribute_GetNum(i_attrType, &count);
	if (err != NoError) {
		WriteReport_Err("ACAPI_Attribute_GetNum", err);
		return 0;
	}

	if (i_func != nullptr)
		for (i = 1; i <= count; i++) {
			BNZeroMemory(&attrib, sizeof(API_Attribute));
			attrib.header.typeID = i_attrType;
			attrib.header.index = i;

			err = ACAPI_Attribute_Get(&attrib);

			if (err == NoError && i_func(attrib, i_attrs))
				result++;
		}
	else
		result = count;

	return result;
}


static GS::Array<DataObject*> ListAttributes(
	API_AttrTypeID i_attrType,
	DataObject* (*i_func)(API_Attribute) = nullptr
)
{
	API_Attribute			attrib;
	API_AttributeIndex		count;
	GSErrCode				err;
	DataObject*				resultThis;
	GS::Array<DataObject*>	io_attrs;

	err = ACAPI_Attribute_GetNum(i_attrType, &count);
	if (err != NoError) {
		WriteReport_Err("ACAPI_Attribute_GetNum", err);
	}

	if (i_func != nullptr)
		for (Int32 i = 1; i <= count; i++) {
			try 
			{
				BNZeroMemory(&attrib, sizeof(API_Attribute));
				attrib.header.typeID = i_attrType;
				attrib.header.index = i;

				err = ACAPI_Attribute_Get(&attrib);

				if (err == NoError)
				{
						resultThis = i_func(attrib);
						io_attrs.Push(resultThis);
				}
			}
			catch (...) {
				continue;
			}
		}

	return io_attrs;
}

static void AddSum(GS::UniString i_sTable ) {
	//TODO
}

static void AddItem(GS::UniString i_sTable, GS::UniString i_sItem, UInt32 i_iItemNumber)
{
	// Adds an item to both the UI report and the .xlsx output
	if (!i_iItemNumber && !cntlDlgData.iAddZeroValues) return;

	char sItem[256], _sNumberOfWalls[256], sInt[256];

	//sprintf(sItem, "Number of %s", i_sItem.ToCStr().Get());
	itoa(i_iItemNumber, sInt, 10);

	if (!cntlDlgData.reportData.ContainsKey(i_sTable))
	{ 
		ReportData _rd{};
		cntlDlgData.reportData.Add(i_sTable, _rd);
	}

	cntlDlgData.reportData[i_sTable].Add(i_sItem, i_iItemNumber);

	sprintf(_sNumberOfWalls, "%s: %s", i_sItem.ToCStr().Get(), sInt);
	DGListInsertItem(32400, 2, DG_LIST_BOTTOM);
	DGListSetItemText(32400, 2, DG_LIST_BOTTOM, GS::UniString(_sNumberOfWalls));
}

static void AddList(GS::UniString i_sTable, GS::UniString i_sItem, UInt32 i_iItemNumber)
{

	if (!cntlDlgData.reportData.ContainsKey(i_sTable))
	{
		ReportData _rd{};
		cntlDlgData.reportData.Add(i_sTable, _rd);
	}

	cntlDlgData.reportData[i_sTable].Add(i_sItem, i_iItemNumber);
}


static short GetChildrenNumber(API_NavigatorItem i_item, 
	const API_NavigatorItemTypeID& i_navID,
	const GS::UniString& i_sInExclude = "",
	const bool i_isInclude = true)
{
	GSErrCode err;
	short result = 0;
	GS::Array<API_NavigatorItem> childItems;

	auto _name = GS::UniString(i_item.uName).ToCStr().Get();

	err = ACAPI_Navigator(APINavigator_GetNavigatorChildrenItemsID, &i_item, nullptr, &childItems);

	if (i_item.itemType == i_navID)
		if (i_sInExclude.GetLength())
			if (i_isInclude == (GS::UniString(i_item.uName).FindFirst(i_sInExclude) < MaxUSize))
				result += 1;
		else
			result += 1;

	for (const API_NavigatorItem& childItem : childItems)
	{
		result += GetChildrenNumber(childItem, i_navID, i_sInExclude, i_isInclude);
	}

	return result;
}


static short GetNavigatorItems(const API_NavigatorMapID& i_mapID, 
	const API_NavigatorItemTypeID& i_navID, 
	const GS::UniString& i_inExcludeString = "", 
	const bool i_isInclude = true)
{
	short result = 0;
	GSErrCode err;

	GS::Array<API_NavigatorItem> childItems;
	API_NavigatorItem item = {};
	API_NavigatorSet	set;
	GS::Array<API_NavigatorSet>	sets;
	BNZeroMemory(&set, sizeof(API_NavigatorSet));
	set.mapId = i_mapID;

	if (i_mapID != API_PublisherSets)
	{
		err = ACAPI_Navigator(APINavigator_GetNavigatorSetID, &set, nullptr);
		sets.Push(set);
	}
	else
	{
		Int32 nSet = 0;
		err = ACAPI_Navigator(APINavigator_GetNavigatorSetNumID, &nSet, nullptr);

		for (Int32 _i = 0; _i < nSet; _i++)
		{
			err = ACAPI_Navigator(APINavigator_GetNavigatorSetID, &set, &_i);
			sets.Push(set);
		}
	}

	for (const auto& _set : sets)
	{
		item.guid = set.rootGuid;
		result += GetChildrenNumber(item, i_navID, i_inExcludeString, i_isInclude);
	}

	return result;
}

// -----------------------------------------------------------------------------
// Open the selected XLSX file into a library part
// -----------------------------------------------------------------------------
static bool	GetOpenFile(IO::Location* dloc, 
	const char* fileExtensions, 
	const GS::UniString& filterText, 
	DG::FileDialog::Type i_type = DG::FileDialog::Type::OpenFile)
{
	if (dloc == nullptr)
		return false;

	FTM::TypeID	retID;
	FTM::FileTypeManager	ftman("MyManager");
	FTM::FileType			type(nullptr, fileExtensions, 0, 0, 0);
	FTM::TypeID				id = FTM::FileTypeManager::SearchForType(type);
	if (id == FTM::UnknownType)
		id = ftman.AddType(type);

	DG::FileDialog	dlg(i_type);

	if (!dlg.Invoke())
		return false;

	*dloc = dlg.GetSelectedFile();
	return true;
}

// -----------------------------------------------------------------------------
// Export GUID and a few basic parameters of all walls placed in the current project.
// -----------------------------------------------------------------------------

#define UNISTR_TO_LIBXLSTR(str) (str.ToUStr ())

static void	Do_ImportNamesFromExcel(void)
{
	IO::Location xlsFileLoc;
	if (!GetOpenFile(&xlsFileLoc, "xlsx", "*.xlsx", DG::FileDialog::OpenFile))
		return;

	GS::UniString filepath;
	xlsFileLoc.ToPath(&filepath);

	libxl::Book* book;
	bool isBookLoaded = false;

	book = xlCreateBook();
	if (book->load(filepath.ToUStr()))
		isBookLoaded = true;
	else
	{
		book = xlCreateXMLBook();
		if (book->load(filepath.ToUStr()))
			isBookLoaded = true;
	}

	if (isBookLoaded)
	{
		if (libxl::Sheet* sheet = book->getSheet(0))
		{
			for (int row = sheet->firstRow(); row < sheet->lastRow(); ++row)
			{
				if (const wchar_t* sFilter = sheet->readStr(row, 0))
					cntlDlgData.filterStrings.Add(GS::UniString(sFilter));
			}
		}

		book->release();
	}
}

static void	Do_ExportReportToExcel(void)
{
	//DBPrintf("Exporting walls to Excel document...\n");

	libxl::Book* book = xlCreateXMLBook();

	for (auto item : cntlDlgData.reportData) {
		const GS::UniString _k = *item.key;
		libxl::Sheet* sheet = book->addSheet(UNISTR_TO_LIBXLSTR(_k));
		sheet->setCol(0, 1, 50.0);

		GS::Array<GS::UniString> titles =
		{ "Object type", "Number" };

		for (UIndex ii = 0; ii < titles.GetSize(); ++ii) {
			sheet->writeStr(0, ii, UNISTR_TO_LIBXLSTR(titles[ii]));
		}

		UIndex ii = 1;

		for (auto iitem : *item.value)
		{
			if (iitem.value > 0 || cntlDlgData.iAddZeroValues)
			{
				sheet->writeStr(ii, 0, iitem.key->ToUStr());
				sheet->writeNum(ii++, 1, *iitem.value);
			}
		}
	}

	IO::Location xlsFileLoc;
	if (!GetOpenFile(&xlsFileLoc, "xlsx", "*.xlsx", DG::FileDialog::Save))
		return;

	GS::UniString filepath;
	xlsFileLoc.ToPath(&filepath);

	DBVERIFY(book->save(UNISTR_TO_LIBXLSTR(filepath)));
	book->release();

	//DBPrintf("Export operation finished\n");
}

bool BoundingBoxesMatch(API_Element& element1, API_Element& element2)
{
	API_Box3D	bbox1, bbox2;
	GSErrCode				err;

	BNZeroMemory(&bbox1, sizeof(bbox1));
	BNZeroMemory(&bbox2, sizeof(bbox2));

	err = ACAPI_Database(APIDb_CalcBoundsID, &element1.header, &bbox1);
	err = ACAPI_Database(APIDb_CalcBoundsID, &element2.header, &bbox2);

	bool xMatch, yMatch, zMatch;
	xMatch = bbox1.xMax > bbox2.xMin && bbox2.xMax > bbox1.xMin;
	yMatch = bbox1.yMax > bbox2.yMin && bbox2.yMax > bbox1.yMin;
	zMatch = bbox1.zMax > bbox2.zMin && bbox2.zMax > bbox1.zMin;
	return xMatch && yMatch && zMatch;
}

GS::HashSet<API_Guid> GetSEOElements(bool isBoundingBoxConsidered = false)
{
	GSErrCode				err;
	API_SelectionInfo		selectionInfo;
	GS::Array<API_Neig>		selNeigs;
	API_Element				elementThis, elementOther;
	GS::Array<API_Guid>		guid_Targets, guid_Operators;
	API_Neig				_neig;
	GS::HashSet<API_Guid>	result;
	GS::Array<API_Guid>		_tempArray;

	err = ACAPI_Element_GetElemList(API_ZombieElemID, &_tempArray);

	for (const API_Guid& _guid : _tempArray) {
		bool bAdd = false;

		BNZeroMemory(&elementThis, sizeof(API_Element));
		elementThis.header.guid = _guid;
		err = ACAPI_Element_Get(&elementThis);
		if (err != NoError) {
			break;
		}

		err = ACAPI_Element_SolidLink_GetTargets(_guid, &guid_Targets);

		err = ACAPI_Element_SolidLink_GetOperators(_guid, &guid_Operators);

		for (const auto& guidTarget : guid_Targets) {
			BNZeroMemory(&elementOther, sizeof(API_Element));
			elementOther.header.guid = guidTarget;

			err = ACAPI_Element_Get(&elementOther);

			if (!isBoundingBoxConsidered || !BoundingBoxesMatch(elementThis, elementOther)) {
				result.Add(elementOther.header.guid);
				bAdd = true;
			}
		}

		for (const auto& guidOperator : guid_Operators) {
			BNZeroMemory(&elementOther, sizeof(API_Element));
			elementOther.header.guid = guidOperator;

			err = ACAPI_Element_Get(&elementOther);

			if (!isBoundingBoxConsidered || !BoundingBoxesMatch(elementThis, elementOther)) {
				result.Add(elementOther.header.guid);
				bAdd = true;
			}
		}

		if (bAdd)
			result.Add(_guid);
	}

	return result;
}

// -----------------------------------------------------------------------------
// Load a localisable Unicode string from resource
// -----------------------------------------------------------------------------

static void		GetStringFromResource(GS::UniString* buffer, short resID, short stringID)
{
	if (buffer != nullptr && !RSGetIndString(buffer, resID, stringID, ACAPI_GetOwnResModule()))
		buffer->Clear();

	return;
}		// GetStringFromResource

static short DGCALLBACK CntlDlgCallBack(short message, short dialID, short item, DGUserData userData, DGMessageData msgData)
{
	short result = 0;
	GS::UniString _text{};
	API_PropertyDefinition _def;

	switch (message) {
	case DG_MSG_INIT:
	{
		GSErrCode err;

		DGSetItemValLong(dialID, ZERO_CHECKBOX, cntlDlgData.iAddZeroValues);


		GS::Array<DataObject*> lLibParts;

		lLibParts = ListLibParts();

		CountLibPartInstances();

		for (auto libPart : iLibPartInstanceS)
		{
			AddItem("Library Part Instances", *libPart.key, *libPart.value);
		}

		//GS::Array<FileSizeReportObject> aEmbedded, aSpecial, aNormal;

		//for (DataObject* lp : lLibParts)
		//{
		//	FileSizeReportObject* _lp = (FileSizeReportObject*)lp;
		//	if (_lp->libType == API_BuiltInLibrary)
		//		aSpecial.Push(*_lp);

		//	if (_lp->libType == API_LocalLibrary
		//		|| _lp->libType == API_ServerLibrary)
		//		aNormal.Push(*_lp);

		//	if (_lp->libType == API_EmbeddedLibrary)
		//		aEmbedded.Push(*_lp);

		//	delete lp;
		//}

		//for (auto item : aNormal)
		//	AddItem("LibPart data", item.name, (UInt32)item.size);

		//for (auto item : aEmbedded)
		//	AddItem("Embedded LibPart data", item.name, (UInt32)item.size);


		//for (UINT16 i = 1; i <= ac_types.GetSize(); i++)
		//{
		//	GS::Array<API_Guid> _array{};
		//	char intStr[256], _sNumberOfWalls[256], _sNumberOfWalls2[256];

		//	err = ACAPI_Element_GetElemList(static_cast<API_ElemTypeID>(i), &_array);

		//	auto _a = ac_types[i-1].ToCStr().Get();
		//	sprintf(_sNumberOfWalls2, "Number of %s", _a);
		//	AddItem("Elements", _sNumberOfWalls2, _array.GetSize());
		//}

		//AddItem("SEO Data", "Number of SEO Operators/Targets", GetSEOElements().GetSize());
		//AddItem("SEO Data", "Number of erroneous SEO Operators/Targets", GetSEOElements(true).GetSize());


		//short iNavItems = 0;

		//for (UInt16 iMT = 1; iMT < ac_mapTypes.GetSize(); iMT++)
		//	for (UInt16 iNIT = 1; iNIT < ac_navItemTypes.GetSize(); iNIT++)
		//	{
		//		iNavItems = GetNavigatorItems(static_cast<API_NavigatorMapID>(iMT), static_cast<API_NavigatorItemTypeID>(iNIT));

		//		if (iNavItems > 0 || cntlDlgData.iAddZeroValues)
		//		{
		//			AddItem(ac_mapTypes[iMT], ac_navItemTypes[iNIT], iNavItems);
		//		}

		//		// -------------------------------------

		//		iNavItems = GetNavigatorItems(static_cast<API_NavigatorMapID>(iMT), static_cast<API_NavigatorItemTypeID>(iNIT), "Story");

		//		if (iNavItems > 0 || cntlDlgData.iAddZeroValues)
		//		{
		//			AddItem(ac_mapTypes[iMT], ac_navItemTypes[iNIT] + "Story", iNavItems);
		//		}
		//	}

		//AddItem("Layer data", "Number of Layers", CountAttributes(API_LayerID));

		//for (auto sFilter: cntlDlgData.filterStrings)
		//{ 
		//	auto iCount = CountAttributes(API_LayerID, nameContains, &StringData{ sFilter });
		//	AddItem("Layer data", "Number of Layers containing the string \"" + sFilter + "\"", iCount);
		//}
		//
		//AddItem("Layer data", "Number of Materials", CountAttributes(API_MaterialID));
		//AddItem("Layer data", "Number of Materials with Texture", CountAttributes(API_MaterialID, hasTexture));

		//GS::Array<DataObject*> lTextures;
		//
		//lTextures = ListAttributes(API_MaterialID, getTextureSize);

		//for (DataObject* tex : lTextures)
		//{
		//	FileSizeReportObject* _tex = (FileSizeReportObject*)tex;
		//	AddItem("Texture data", _tex->name, (UInt16)_tex->size);
		//	delete tex;
		//}

		//Int32 iLibParts;


		break;
	}
	case DG_MSG_CLICK:
		switch (item) {
		case OK_BUTTON:
			result = item;
			break;
		case EXPORT_BUTTON:
			Do_ExportReportToExcel();

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

	switch (message) {
	case DG_MSG_INIT:
	{
		GSErrCode err;

		DGSetItemValLong(dialID, ZERO_CHECKBOX, cntlDlgData.iAddZeroValues);

		break;
	}
	case DG_MSG_CLICK:
		switch (item) {
		case OK_BUTTON:
			result = item;
			break;
		case IMPORT_BUTTON:
			Do_ImportNamesFromExcel();

			result = item;
			break;
		}
	case DG_MSG_CHANGE:
		switch (item) {
		case ZERO_CHECKBOX:
			//TODO Refresh the dialog
			cntlDlgData.iAddZeroValues = DGGetItemValLong(dialID, ZERO_CHECKBOX);
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
	apiTypeDict = APITypeDict{};

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

