#include	"APIEnvir.h"
#include	"ACAPinc.h"					// also includes APIdefs.h
#include	"APICommon.h"
#include	"APITypeDict.hpp"


APITypeDict::APITypeDict()
{
	GS::Array<API_LibraryInfo>    libInfo;
	IO::Path    sEmbeddedLibPath;

	if (ACAPI_Environment(APIEnv_GetLibrariesID, &libInfo) == NoError) {
		for (API_LibraryInfo lib : libInfo) {
			lib.location.ToPath(&sEmbeddedLibPath);

			table.Add(GS::UniString(sEmbeddedLibPath), lib.libraryType);
		}
	}
}

API_LibraryTypeID APITypeDict::GetLibPartType(const API_LibPart& i_libPart) const 
{
	GS::UniString path = "";
	API_LibraryTypeID result = API_Undefined;
	i_libPart.location->ToPath(&path);
	GS::UniString sPath = GS::UniString(path);

	for (auto k : table.Keys())
	{
		if (sPath.Contains(k))
		{
			return table[k];
		}
	}

	return result;
}

