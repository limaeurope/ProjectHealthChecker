#ifndef _ATTRIBUTE_HPP
#define _ATTRIBUTE_HPP

#include	"APIEnvir.h"
#include	"ACAPinc.h"					// also includes APIdefs.h
#include	"APICommon.h"
#include	"DataStructs.hpp"

// -----------------------------------------------------------------------------
//  List attributes
// -----------------------------------------------------------------------------

UInt32 CountAttributes(
	API_AttrTypeID i_attrType,
	bool(*i_func)(API_Attribute, AbstractData*) = nullptr,
	AbstractData* i_attrs = nullptr);

GS::Array<DataObject*> ListAttributes(
	API_AttrTypeID i_attrType,
	DataObject* (*i_func)(API_Attribute) = nullptr);

#endif	//_ATTRIBUTE_HPP

