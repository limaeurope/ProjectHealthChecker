#ifndef ATTRIBUTE_USAGE_HPP
#define ATTRIBUTE_USAGE_HPP

#include	"APIEnvir.h"
#include	"ACAPinc.h"					// also includes APIdefs.h
#include	"APICommon.h"


class AttributeUsage
{
public:
	GS::HashTable<API_AttributeIndex, int> layerContentTable;
	AttributeUsage();
};

#endif // !ATTRIBUTE_USAGE_HPP

