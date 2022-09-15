
#ifndef _LIBPART_HPP
#define _LIBPART_HPP

#include	"APIEnvir.h"
#include	"ACAPinc.h"					// also includes APIdefs.h
#include	"APICommon.h"
#include	"DataStructs.hpp"
#include	"APITypeDict.hpp"

GS::Array<DataObject*> ListLibParts(APITypeDict& i_apiTypeDict);

inline GS::UniString GetLibPartName(const API_LibPart& i_libPart);

void CountLibPartInstances(GS::HashTable<GS::UniString, UInt32> iLibPartInstanceS);

#endif //_LIBPART_HPP