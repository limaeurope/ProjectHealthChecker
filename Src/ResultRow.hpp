#ifndef _RESULTROW_HPP
#define _RESULTROW_HPP

#include	"APIEnvir.h"
#include	"ACAPinc.h"					// also includes APIdefs.h
#include	"APICommon.h"


class ResultRow
{
	GS::HashTable<UInt16, UInt32>	fields;
public:
	ResultRow(UInt16 i_col, UInt32 i_value) { Add(i_col, i_value); };
	ResultRow(UInt32 i_value) { Add(0, i_value); };
	ResultRow(GS::Array<UInt32> i_valueS);
	//ResultRow(ResultRow&) {};
	ResultRow() {};

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

	inline void operator= (GS::HashTable<UInt16, UInt32>  i_valueS) { fields = i_valueS; };
	UInt32 operator[](UInt16 i_index);
	ResultRow operator+ (const ResultRow& i_other);
};

#endif	//_RESULTROW_HPP
