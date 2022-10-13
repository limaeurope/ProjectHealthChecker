#ifndef _SEO_HPP
#define _SEO_HPP

#include	"../APIEnvir.h"
#include	"ACAPinc.h"					// also includes APIdefs.h
#include	"../APICommon.h"

bool BoundingBoxesMatch(const API_Element& element1, const API_Element& element2);

GS::HashSet<API_Guid> GetSEOElements(bool isBoundingBoxConsidered = false);

#endif	//_SEO_HPP