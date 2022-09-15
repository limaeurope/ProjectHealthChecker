#include	"Attribute.hpp"

// -----------------------------------------------------------------------------
//  List attributes
// -----------------------------------------------------------------------------

UInt32 CountAttributes(
	API_AttrTypeID i_attrType,
	bool(*i_func)(API_Attribute, AbstractData*) /*= nullptr*/,
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


GS::Array<DataObject*> ListAttributes(
	API_AttrTypeID i_attrType,
	DataObject* (*i_func)(API_Attribute) /*= nullptr*/)
{
	API_Attribute			attrib;
	API_AttributeIndex		count;
	GSErrCode				err;
	DataObject* resultThis;
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

