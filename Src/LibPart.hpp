
#ifndef _LIBPART_HPP
#define _LIBPART_HPP

#include	"APIEnvir.h"
#include	"ACAPinc.h"					// also includes APIdefs.h
#include	"APICommon.h"
#include	"DataStructs.hpp"
#include	"APITypeDict.hpp"


void ProcessLibPars(CntlDlgData& io_cntlDlgData,
	const APITypeDict& i_apiTypeDict,
	GS::HashTable<GS::UniString, UInt32> io_iLibPartInstanceS);

inline GS::Array<AbstractData*> ListLibParts(const APITypeDict& i_apiTypeDict);

void CountLibPartInstances(GS::HashTable<GS::UniString, UInt32> iLibPartInstanceS);

#endif //_LIBPART_HPP