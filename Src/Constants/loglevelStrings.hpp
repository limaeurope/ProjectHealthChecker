#ifndef	_LOGLEVEL_STRINGS_
#define	_LOGLEVEL_STRINGS_

#include	"APIEnvir.h"
#include	"ACAPinc.h"					// also includes APIdefs.h
#include	"APICommon.h"


static const GS::Array<GS::UniString> sLoglevels{
	"TRACE  ",
	"DEBUG  ",
	"INFO   ",
	"WARNING",
	"ERROR  ",
	"FATAL  ",
};
#endif // _LOGLEVEL_STRINGS_

