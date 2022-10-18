#ifndef _STRINGS_ENUM_HPP
#define	_STRINGS_ENUM_HPP
#include	"APIEnvir.h"
#include	"ACAPinc.h"					// also includes APIdefs.h
#include	"APICommon.h"

const UInt16 TABLE_DATA = 32603;

typedef enum {
	NumObj = 1,
	NumAttr,
	NumLib,
	LayerData,
	LayerName,
	NumObjOnLayer,
	NumLay,
	NumLayContainString,
	PenData,
	PenNumber,
	LTData,
	LTName,
	FillData,
	FillName,
	SurfData,
	SurfName,
	BuildMatName,
	BuildMatData,
	CompName,
	CompData,
	ProfName,
	ProfData,
	ZoneName,
	ZoneData,
	MatName,
	MatData,
	MatNum,
	MatNumWithText,
	TexName,
	TexData,
	SizeInB,

	LibPartData,
	LibPartDataEmb,
	LibPartInst,
	LibPartPath,
	NumOfInst,

	Elements,

} IntStr;

#endif // !_STRINGS_ENUM_HPP

