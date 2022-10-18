#include	"Attribute.hpp"
#include	"SettingsSingleton.hpp"
#include	"APIdefs_Elements.h"
#include	"APIdefs_Attributes.h"
#include	"DGFileDialog.hpp"
#include	"../Enums/StringsEnum.hpp"
#include	"../Utils/Utils.hpp"


// -----------------------------------------------------------------------------
//  As parameter called functions
// -----------------------------------------------------------------------------

bool HasTexture(const API_Attribute& i_apiAttrib, AbstractData* const i_attrs)
{
	UNUSED_PARAMETER(i_attrs);
	return i_apiAttrib.material.texture.fileLoc != NULL;
}

bool NameContains(const API_Attribute& i_apiAttrib, AbstractData* const i_attrs)
{
	return GS::UniString(i_apiAttrib.header.name)
		.FindFirst(static_cast<StringData*>(i_attrs)->string) < MaxUIndex;
}

AbstractData* GetLayerName(const API_Attribute& i_apiAttrib, AbstractData* const i_attrs)
{
	LayerReportObject* result = new LayerReportObject;

	result->name = i_apiAttrib.header.name;

	return result;
}

AbstractData* GetTextureSize(const API_Attribute& i_apiAttrib, AbstractData* const i_attrs)
{
	UNUSED_PARAMETER(i_attrs);
	if (i_apiAttrib.material.texture.status == 0)
		throw 1;
	IO::Location loc{ *i_apiAttrib.material.texture.fileLoc };
	IO::File f{ loc };
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

		result->index = i_apiAttrib.header.index;

		return result;
	}
	else throw 1;	//TODO
}

AbstractData* CountLayerContents(const API_Attribute& i_apiAttrib, AbstractData* const i_attrs)
{
	LayerReportObject* result = new LayerReportObject;

	const auto idx = i_apiAttrib.layer.head.index;
	auto attrs = (AttributeData*)(i_attrs);
	auto lct = attrs->attributeUsage.layerContentTable;

	if (lct.ContainsKey(idx))
	{
		result->nInstances = lct[idx][0];
		result->name = GS::UniString(i_apiAttrib.layer.head.name);
	}

	return result;
}

template <typename T>
AbstractData* CountAttributeContents(const API_Attribute& i_apiAttrib, AbstractData* const i_attrs)
{
	AttributeReportObject* result = new AttributeReportObject;

	GS::HashTable<T, ResultRow>  t = ((StructDefObject<T>*)i_attrs)->table;
	T i = (T)i_apiAttrib.header.index;
	result->id = i;
	result->name = GS::UniString(i_apiAttrib.header.name);
	if (t.ContainsKey(i))
	{
		result->resultRow = t[i];
	}
	else
		result->resultRow = ResultRow { 0 };

	return result;
}

// -----------------------------------------------------------------------------

template<typename T>
void AddAttributeListToTable(const API_AttrTypeID i_attrID, const GS::HashTable<T, ResultRow>& i_table, const GS::UniString& i_sTable, const bool i_hasName = true)
{
	GS::Array<AbstractData*> resultArray;

	resultArray = ListAttributes(i_attrID, CountAttributeContents<T>, &StructDefObject<T> (i_table));

	for (AbstractData* attrib: resultArray)
	{
		AttributeReportObject* _attrib = (AttributeReportObject*)attrib;

		if (i_hasName)
			SETTINGS().GetSheet(i_sTable).AddItem(GS::UniString(_attrib->name), _attrib->resultRow);
		else
		{
			char sAttrib[4];
			itoa(_attrib->id, sAttrib, 10);
			SETTINGS().GetSheet(i_sTable).AddItem(GS::UniString(sAttrib), _attrib->resultRow);
		}
	}
}

template<typename T>
void AddAttributeListToTable(const API_AttrTypeID i_attrID, const GS::HashTable<T, ResultRow>& i_table, const IntStr i_sTable, const bool i_hasName = true)
{
	return AddAttributeListToTable(i_attrID, i_table, GSFR(i_sTable), i_hasName);
}

// -----------------------------------------------------------------------------
//  List attributes
// -----------------------------------------------------------------------------

void ProcessAttributes(AttributeUsage& i_attributeUsage)
{
	SETTINGS().GetSheet(LayerData).SetHeader(ReportSheetHeader{ GSFR(LayerName), GSFR(NumObjOnLayer) });
	SETTINGS().GetSheet(LayerData).AddItem(GSFR(NumLay), CountAttributes(API_LayerID));

	for (auto &sFilter : SETTINGS().FilterStrings)
	{
		UInt32 iCount = CountAttributes(API_LayerID, NameContains, &StringData{ sFilter });
		SETTINGS().GetSheet(LayerData).AddItem(GSFR(NumLayContainString) + sFilter + "\"", iCount);
	}

	#define REPORT_ROWS GSFR(NumObj), GSFR(NumAttr), GSFR(NumLib)

	AddAttributeListToTable<API_AttributeIndex>(API_LayerID, i_attributeUsage.layerContentTable, LayerData);
	SETTINGS().GetSheet(LayerData).SetHeader(ReportSheetHeader{ GSFR(LayerName), REPORT_ROWS });

	AddAttributeListToTable<short>(API_PenID, i_attributeUsage.penUsageTable, PenData, false);
	SETTINGS().GetSheet(PenData).SetHeader(ReportSheetHeader{ GSFR(PenNumber), REPORT_ROWS});

	AddAttributeListToTable<API_AttributeIndex>(API_LinetypeID, i_attributeUsage.ltUsageTable, LTData);
	SETTINGS().GetSheet(LTData).SetHeader(ReportSheetHeader{ GSFR(LTName), REPORT_ROWS});

	AddAttributeListToTable<API_AttributeIndex>(API_FilltypeID, i_attributeUsage.fillUsageTable, FillData);
	SETTINGS().GetSheet(FillData).SetHeader(ReportSheetHeader{ GSFR(FillName), REPORT_ROWS});

	AddAttributeListToTable<API_AttributeIndex>(API_MaterialID, i_attributeUsage.surfUsageTable, SurfData);
	SETTINGS().GetSheet(SurfData).SetHeader(ReportSheetHeader{ GSFR(SurfName), REPORT_ROWS});

	AddAttributeListToTable<API_AttributeIndex>(API_BuildingMaterialID, i_attributeUsage.buildMatUsageTable, BuildMatData);
	SETTINGS().GetSheet(BuildMatData).SetHeader(ReportSheetHeader{ GSFR(BuildMatName), REPORT_ROWS});

	AddAttributeListToTable<API_AttributeIndex>(API_CompWallID, i_attributeUsage.compositeUsageTable, CompData);
	SETTINGS().GetSheet(CompData).SetHeader(ReportSheetHeader{ GSFR(CompName), REPORT_ROWS});

	AddAttributeListToTable<API_AttributeIndex>(API_ProfileID, i_attributeUsage.profileUsageTable, ProfData);
	SETTINGS().GetSheet(ProfData).SetHeader(ReportSheetHeader{ GSFR(ProfName), REPORT_ROWS});

	AddAttributeListToTable<API_AttributeIndex>(API_ZoneCatID, i_attributeUsage.zoneUsageTable, ZoneData);
	SETTINGS().GetSheet(ZoneData).SetHeader(ReportSheetHeader{ GSFR(ZoneName), REPORT_ROWS});

	SETTINGS().GetSheet(MatData).AddItem(MatNum, CountAttributes(API_MaterialID));
	SETTINGS().GetSheet(MatData).AddItem(MatNumWithText, CountAttributes(API_MaterialID, HasTexture));


	GS::Array<AbstractData*> lTextures;

	lTextures = ListAttributes(API_MaterialID, GetTextureSize);

	for (AbstractData* tex : lTextures)
	{
		FileSizeReportObject* _tex = (FileSizeReportObject*)tex;
		ResultRow _row;

		if (i_attributeUsage.surfUsageTable.ContainsKey(_tex->index))
			_row = i_attributeUsage.surfUsageTable[_tex->index];
		else
			_row = ResultRow{ 0 };

		SETTINGS().GetSheet(TexData).AddItem(_tex->name, ResultRow{ (UInt32)_tex->size} + _row);
	}

	SETTINGS().GetSheet(TexData).SetHeader(ReportSheetHeader{ "Texture name", "Size in Bytes", REPORT_ROWS });
}

UInt32 CountAttributes(
	const API_AttrTypeID i_attrType,
	bool(*const i_func)(const API_Attribute&, AbstractData* const) /*= nullptr*/,
	AbstractData* const i_attrs /*= nullptr*/)
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

	if (i_func)
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

GS::Array<AbstractData*> ListAttributes(
	const API_AttrTypeID i_attrType,
	AbstractData* (*const i_func)(const API_Attribute&, AbstractData* const) /*= nullptr*/,
	AbstractData* const i_attrs /*= nullptr*/)
{
	API_Attribute			attrib;
	API_AttributeIndex		count;
	GSErrCode				err;
	AbstractData* resultThis;
	GS::Array<AbstractData*>	io_attrs;

	err = ACAPI_Attribute_GetNum(i_attrType, &count);
	if (err != NoError) {
		WriteReport_Err("ACAPI_Attribute_GetNum", err);
	}

	if (i_func)
		for (Int32 i = 1; i <= count; i++) {
			try
			{
				BNZeroMemory(&attrib, sizeof(API_Attribute));
				attrib.header.typeID = i_attrType;
				attrib.header.index = i;

				err = ACAPI_Attribute_Get(&attrib);

				if (err == NoError)
				{
					resultThis = i_func(attrib, i_attrs);
					io_attrs.Push(resultThis);
				}
			}
			catch (...) {
				continue;
			}
		}

	return io_attrs;
}

