
#ifndef _LIBPART_HPP
#define _LIBPART_HPP

#include	"../APIEnvir.h"
#include	"ACAPinc.h"					// also includes APIdefs.h
#include	"../APICommon.h"
#include	"../Data/DataStructs.hpp"


void ProcessLibParts();

inline GS::Array<AbstractData*> ListLibParts();

void CountLibPartInstances(GS::HashTable<GS::UniString, UInt32>* iLibPartInstanceS);

#endif //_LIBPART_HPP