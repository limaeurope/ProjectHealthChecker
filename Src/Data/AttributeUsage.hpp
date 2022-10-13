#ifndef ATTRIBUTE_USAGE_HPP
#define ATTRIBUTE_USAGE_HPP

#include	"APIEnvir.h"
#include	"ACAPinc.h"					// also includes APIdefs.h
#include	"APICommon.h"
#include	"ResultRow.hpp"

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
	void Append(const AttributeUsageSet& i_other);
	inline void operator+(AttributeUsageSet& i_other) {Append(i_other);};
	inline void operator+=(AttributeUsageSet& i_other) {Append(i_other);};
	GS::USize GetSize() const;
	AttributeUsageSet(const AttributeUsage* const i_attributeUsage);
	AttributeUsageSet() {};
};


class AttributeUsage
{
	void LayerUsage(const API_Element& i_element);
	AttributeUsageSet GeneralAttributeUsageByElement(const API_Element& i_element);
	AttributeUsageSet GeneralAttributeUsageByAttribute(const API_Attribute& i_attribute, const API_AttributeDefExt& i_defs, const API_AttrTypeID i_type);
	template <typename T>
	void ProcessAttributeUsage(const GS::HashSet <T>& i_attributeSet, GS::HashTable<T, int>& io_attributeUsageTable);
	void GeneralAttributeUsageController();
	template<typename T>
	void ProcessAttributeSet(const GS::HashSet<T>& i_set, const API_AttrTypeID i_type, AttributeUsageSet& io_todoSet);
	void ProcessParameters(const API_Element& i_element, AttributeUsageSet* const io_attributeUsageSet);
	void ProcessVectorImage(const API_AttributeDefExt& i_defs,
		AttributeUsageSet* const i_newSet,
		AttributeUsageSet* const i_todoSet);
public:
	GS::HashTable<API_AttributeIndex, ResultRow> layerContentTable;
	GS::HashTable<short, ResultRow> penUsageTable;
	GS::HashTable<API_AttributeIndex, ResultRow> ltUsageTable;
	GS::HashTable<API_AttributeIndex, ResultRow> fillUsageTable;
	GS::HashTable<API_AttributeIndex, ResultRow> surfUsageTable;
	GS::HashTable<API_AttributeIndex, ResultRow> buildMatUsageTable;
	GS::HashTable<API_AttributeIndex, ResultRow> compositeUsageTable;
	GS::HashTable<API_AttributeIndex, ResultRow> profileUsageTable;
	GS::HashTable<API_AttributeIndex, ResultRow> zoneUsageTable;

	AttributeUsage();
};



#endif // !ATTRIBUTE_USAGE_HPP

