#ifndef _ATTRIBUTE_HPP
#define _ATTRIBUTE_HPP

#include	"../APIEnvir.h"
#include	"ACAPinc.h"					// also includes APIdefs.h
#include	"../APICommon.h"
#include	"../DataStructs.hpp"

bool HasTexture(const API_Attribute& i_apiAttrib, AbstractData* i_attr);

bool NameContains(const API_Attribute& i_apiAttrib, AbstractData* i_attrs);

AbstractData* GetTextureSize(const API_Attribute& i_apiAttrib, AbstractData* i_attr);

// -----------------------------------------------------------------------------
//  List attributes
// -----------------------------------------------------------------------------

void ProcessAttributes();

UInt32 CountAttributes(
	const API_AttrTypeID i_attrType,
	bool(*i_func)(const API_Attribute&, AbstractData*) = nullptr,
	AbstractData* i_attrs = nullptr);

GS::Array<AbstractData*> ListAttributes(
	const API_AttrTypeID i_attrType,
	AbstractData* (*i_func)(const API_Attribute&, AbstractData*) = nullptr);

#endif	//_ATTRIBUTE_HPP

