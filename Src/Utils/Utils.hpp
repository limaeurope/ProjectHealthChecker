#ifndef _UTILS_HPP
#define _UTILS_HPP

#include	"../APIEnvir.h"
#include	"ACAPinc.h"					// also includes APIdefs.h
#include	"../APICommon.h"
#include	"../Enums/StringsEnum.hpp"

extern void		GetStringFromResource(GS::UniString* buffer, short resID, short stringID);
extern GS::UniString	GetStringFromResource_(short resID, short stringID);
extern GS::UniString GSFR(IntStr stringID);

#endif //_UTILS_HPP

