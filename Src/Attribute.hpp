#ifndef _ATTRIBUTE_HPP
#define _ATTRIBUTE_HPP

#include	"APIEnvir.h"
#include	"ACAPinc.h"					// also includes APIdefs.h
#include	"APICommon.h"
#include	"DataStructs.hpp"

bool hasTexture(const API_Attribute& i_apiAttrib, AbstractData* i_attr);

bool nameContains(const API_Attribute& i_apiAttrib, AbstractData* i_attrs);

AbstractData* getTextureSize(const API_Attribute& i_apiAttrib, AbstractData* i_attr);

// -----------------------------------------------------------------------------
//  List attributes
// -----------------------------------------------------------------------------

void ProcessAttributes(CntlDlgData& io_cntlDlgData);

UInt32 CountAttributes(
	const API_AttrTypeID i_attrType,
	bool(*i_func)(const API_Attribute&, AbstractData*) = nullptr,
	AbstractData* i_attrs = nullptr);

GS::Array<AbstractData*> ListAttributes(
	const API_AttrTypeID i_attrType,
	AbstractData* (*i_func)(const API_Attribute&, AbstractData*) = nullptr);

#endif	//_ATTRIBUTE_HPP

