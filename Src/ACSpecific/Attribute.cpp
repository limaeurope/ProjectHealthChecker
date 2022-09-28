#include	"Attribute.hpp"
#include	"Table.hpp"
#include	"SettingsSingleton.hpp"
#include	"APIdefs_Elements.h"
#include	"APIdefs_Attributes.h"


bool HasTexture(const API_Attribute& i_apiAttrib, AbstractData* i_attrs)
{
	UNUSED_PARAMETER(i_attrs);
	return i_apiAttrib.material.texture.fileLoc != NULL;
}

bool NameContains(const API_Attribute& i_apiAttrib, AbstractData* i_attrs)
{
	return GS::UniString(i_apiAttrib.header.name)
		.FindFirst(static_cast<StringData*>(i_attrs)->string) < MaxUIndex;
}

AbstractData* GetTextureSize(const API_Attribute& i_apiAttrib, AbstractData* i_attrs)
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

		return result;
	}
	else throw 1;	//TODO
}

AbstractData* CountLayerContents(const API_Attribute& i_apiAttrib, AbstractData* i_attrs)
{
	LayerReportObject* result = new LayerReportObject;

	const auto idx = i_apiAttrib.layer.head.index;
	//auto attrs = static_cast<AttributeData*>(i_attrs);
	auto attrs = (AttributeData*)(i_attrs);
	auto lct = attrs->attributeUsage.layerContentTable;

	if (lct.ContainsKey(idx))
	{
		result->nInstances = lct[idx];
		result->name = GS::UniString(i_apiAttrib.layer.head.name);
	}

	return result;
}


// -----------------------------------------------------------------------------
//  List attributes
// -----------------------------------------------------------------------------

void ProcessAttributes(AttributeUsage& i_attributeUsage)
{
	AddItem("Layer data", "Number of Layers", CountAttributes(API_LayerID));

	for (auto sFilter : SETTINGS().FilterStrings)
	{
		auto iCount = CountAttributes(API_LayerID, NameContains, &StringData{ sFilter });
		AddItem("Layer data", "Number of Layers containing the string \"" + sFilter + "\"", iCount);
	}

	GS::Array<AbstractData*> lLayers;

	AttributeData ad{ i_attributeUsage };

	lLayers = ListAttributes(API_LayerID, CountLayerContents, &ad);

	for (AbstractData* lay : lLayers)
	{
		LayerReportObject* _lay = (LayerReportObject*)lay;
		AddItem("Texture data", _lay ->name, (UInt16)_lay->nInstances);
	}

	AddItem("Material data", "Number of Materials", CountAttributes(API_MaterialID));
	AddItem("Material data", "Number of Materials with Texture", CountAttributes(API_MaterialID, HasTexture));

	GS::Array<AbstractData*> lTextures;

	lTextures = ListAttributes(API_MaterialID, GetTextureSize);

	for (AbstractData* tex : lTextures)
	{
		FileSizeReportObject* _tex = (FileSizeReportObject*)tex;
		AddItem("Texture data", _tex->name, (UInt16)_tex->size);
		delete tex;
	}
}

UInt32 CountAttributes(
	const API_AttrTypeID i_attrType,
	bool(*i_func)(const API_Attribute&, AbstractData*) /*= nullptr*/,
	AbstractData* i_attrs /*= nullptr*/)
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

GS::Array<AbstractData*> ListAttributes(
	const API_AttrTypeID i_attrType,
	AbstractData* (*i_func)(const API_Attribute&, AbstractData*) /*= nullptr*/,
	AbstractData* i_attrs /*= nullptr*/)
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

