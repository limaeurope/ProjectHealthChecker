#ifndef _ATTRIBUTE_HPP
#define _ATTRIBUTE_HPP

#include	"APIEnvir.h"
#include	"ACAPinc.h"					// also includes APIdefs.h
#include	"APICommon.h"
#include	"DataStructs.hpp"
#include	"AttributeUsage.hpp"

bool HasTexture(const API_Attribute& i_apiAttrib, AbstractData* const i_attr);

bool NameContains(const API_Attribute& i_apiAttrib, AbstractData* const i_attrs);

AbstractData* GetTextureSize(const API_Attribute& i_apiAttrib, AbstractData* const i_attr);

// -----------------------------------------------------------------------------
//  List attributes
// -----------------------------------------------------------------------------

void ProcessAttributes();

UInt32 CountAttributes(
	const API_AttrTypeID i_attrType,
	bool(*const i_func)(const API_Attribute&, AbstractData* const) = nullptr,
	AbstractData* const i_attrs = nullptr);

GS::Array<AbstractData*> ListAttributes(
	const API_AttrTypeID i_attrType,
	AbstractData* (*const i_func)(const API_Attribute&, AbstractData* const) = nullptr,
	AbstractData* const i_attrs = nullptr);

#endif	//_ATTRIBUTE_HPP

