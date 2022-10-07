#ifndef ATTRIBUTE_USAGE_HPP
#define ATTRIBUTE_USAGE_HPP

#include	"APIEnvir.h"
#include	"ACAPinc.h"					// also includes APIdefs.h
#include	"APICommon.h"
#include	"ReportRow.hpp"

class AttributeUsageSet
{
	friend class AttributeUsage;
	GS::HashSet <short> penSet{};
	GS::HashSet <API_AttributeIndex> ltSet{};
	GS::HashSet <API_AttributeIndex> fillSet{};
	GS::HashSet <API_AttributeIndex> buildMatSet{};
	GS::HashSet <API_AttributeIndex> compSet{};
	GS::HashSet <API_AttributeIndex> surfSet{};
	GS::HashSet <API_AttributeIndex> profSet{};
public:
	void Append(AttributeUsageSet& i_other);
	inline void operator+(AttributeUsageSet& i_other) {Append(i_other);};
	inline void operator+=(AttributeUsageSet& i_other) {Append(i_other);};
	GS::USize GetSize();
	AttributeUsageSet(AttributeUsage* i_attributeUsage);
	AttributeUsageSet() {};
};


class AttributeUsage
{
	void LayerUsage(API_Element i_element);
	AttributeUsageSet GeneralAttributeUsageByElement(API_Element i_element);
	AttributeUsageSet GeneralAttributeUsageByAttribute(API_Attribute i_attribute, API_AttributeDefExt i_defs, API_AttrTypeID i_type);
	template <typename T>
	void ProcessAttributeUsage(const GS::HashSet <T>& i_attributeSet, GS::HashTable<T, int>& io_attributeUsageTable);
	void GeneralAttributeUsageController();
	template<typename T>
	void ProcessAttributeSet(const GS::HashSet<T>& i_set, API_AttrTypeID i_type, AttributeUsageSet& io_todoSet);
	void ProcessParameters(API_Element i_element, AttributeUsageSet* io_attributeUsageSet);
public:
	GS::HashTable<API_AttributeIndex, ReportRow> layerContentTable;
	GS::HashTable<short, ReportRow> penUsageTable;
	GS::HashTable<API_AttributeIndex, ReportRow> ltUsageTable;
	GS::HashTable<API_AttributeIndex, ReportRow> fillUsageTable;
	GS::HashTable<API_AttributeIndex, ReportRow> surfUsageTable;
	GS::HashTable<API_AttributeIndex, ReportRow> buildMatUsageTable;
	GS::HashTable<API_AttributeIndex, ReportRow> compositeUsageTable;
	GS::HashTable<API_AttributeIndex, ReportRow> profileUsageTable;
	GS::HashTable<API_AttributeIndex, ReportRow> zoneUsageTable;
	AttributeUsage();
};



#endif // !ATTRIBUTE_USAGE_HPP

