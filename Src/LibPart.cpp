#include	"LibPart.hpp"
#include	"File.hpp"

// -----------------------------------------------------------------------------
//  List libparts
// -----------------------------------------------------------------------------

GS::Array<DataObject*> ListLibParts(APITypeDict& i_apiTypeDict)
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
		for (auto lib : libInfo) {
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

					result->libType = i_apiTypeDict.GetLibPartType(libPart);

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

void CountLibPartInstances(GS::HashTable<GS::UniString, UInt32> io_iLibPartInstanceS)
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
			io_iLibPartInstanceS.Add(GetLibPartName(libPart), 0);
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
			if (element.header.typeID == API_ObjectID
				|| element.header.typeID == API_LampID)
			{
				libPart.index = element.object.libInd;
				err = ACAPI_LibPart_Get(&libPart);
			}
			else if (element.header.typeID == API_DoorID
				|| element.header.typeID == API_WindowID
				|| element.header.typeID == API_SkylightID)
			{
				err = ACAPI_Goodies(APIAny_GetElemLibPartUnIdID, &element.header, libPart.ownUnID);
				err = ACAPI_LibPart_Search(&libPart, false);
			}
			if (err) throw err;

			if (io_iLibPartInstanceS.ContainsKey(sLibPart = (GetLibPartName(libPart))))
			{
				io_iLibPartInstanceS[sLibPart]++;
			}
			else
			{
				io_iLibPartInstanceS.Add(sLibPart, 1);
			}
		}
		catch (...)
		{
			continue;
		}
	}
}
