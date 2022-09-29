#ifndef ATTRIBUTE_USAGE_HPP
#define ATTRIBUTE_USAGE_HPP

#include	"APIEnvir.h"
#include	"ACAPinc.h"					// also includes APIdefs.h
#include	"APICommon.h"


class AttributeUsage
{
	void LayerUsage(API_Element i_element);
	void GeneralAttributeUsage(API_Element i_element);
	template <typename T>
	void ProcessAttributeUsage(const GS::HashSet <T>& i_attributeSet, GS::HashTable<T, int>& io_attributeUsageTable);
public:
	GS::HashTable<API_AttributeIndex, int> layerContentTable;
	GS::HashTable<short, int> penUsageTable;
	GS::HashTable<API_AttributeIndex, int> ltUsageTable;
	GS::HashTable<API_AttributeIndex, int> fillUsageTable;
	GS::HashTable<API_AttributeIndex, int> surfUsageTable;
	GS::HashTable<API_AttributeIndex, int> buildMatUsageTable;
	GS::HashTable<API_AttributeIndex, int> compositeUsageTable;
	GS::HashTable<API_AttributeIndex, int> profileUsageTable;
	GS::HashTable<API_AttributeIndex, int> zoneUsageTable;
	AttributeUsage();
};



#endif // !ATTRIBUTE_USAGE_HPP

