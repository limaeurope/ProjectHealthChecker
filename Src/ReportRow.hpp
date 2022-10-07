#ifndef _REPORTROW_HPP
#define _REPORTROW_HPP

#include	"APIEnvir.h"
#include	"ACAPinc.h"					// also includes APIdefs.h
#include	"APICommon.h"


class ReportRow
{
	GS::HashTable<UInt16, UInt32>	fields;
public:
	inline void Add(UInt16 i_col, UInt32 i_value) { fields.Add(i_col, i_value); };
	inline void Add(UInt32 i_value) { fields.Add(0, i_value); };
	void Add(GS::Array<UInt32> i_valueS);
	UInt16 Get(UInt16 i_col = 0) const;
	GS::Array<UInt32> GetRow(UInt16 i_maxPos = 3);
	GS::UInt16 GetSize() { return fields.GetSize(); };
	const char* ToStr() const;
	const GS::Array<UInt32> ToArray() const;
	void Inc(UInt16 i_idx);
	GS::HashTable<UInt16, UInt32>::Iterator Begin(){ return fields.Begin();};
	GS::HashTable<UInt16, UInt32>::Iterator End(){ return fields.End();};

	ReportRow(UInt16 i_col, UInt32 i_value) { Add(i_col, i_value); };
	ReportRow(UInt32 i_value) { Add(0, i_value); };
	ReportRow(GS::Array<UInt32> i_valueS) { Add(i_valueS); };
	ReportRow() {};

	inline void operator= (GS::HashTable<UInt16, UInt32>  i_valueS) { fields = i_valueS; };
	UInt32 operator[](UInt16 i_index);
};

#endif	//_REPORTROW_HPP
