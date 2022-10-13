#include	"ResultRow.hpp"

GS::Array<UInt32> ResultRow::GetRow(const UInt16 i_maxPos)
{
	UInt16 maxVal = 0;
	GS::Array<UInt32> resultArray;

	for (auto k : fields.Keys())
		maxVal = max(maxVal, k);

	for (UInt16 i = 0; i <= maxVal; i++)
	{
		if (fields.ContainsKey(i))
			resultArray.Push(fields[i]);
		else
			resultArray.Push(0);
	}

	return resultArray;
}

const char* ResultRow::ToStr() const
{
	char _p[256];
	GS::UniString _us("");
	for (auto _s : fields.Keys())
	{
		itoa(fields[_s], _p, 10);
		_us += GS::UniString(_p) + " ";
	}
	_us.TrimRight();
	return static_cast <const char*>(_us.ToCStr());
}

const GS::Array<UInt32> ResultRow::ToArray() const
{
	GS::Array<UInt32> result;

	for (auto k : fields.Keys())
	{
		for (UInt16 _i = result.GetSize(); _i <= k; _i++)
			result.Push(0);
		result[k] = fields[k];
	}

	return result;
}

void ResultRow::Inc(UInt16 i_idx)
{
	if (fields.ContainsKey(i_idx))
		fields[i_idx]++;
	else
		fields.Add(i_idx, 1);
}

ResultRow::ResultRow(GS::Array<UInt32> i_valueS)
{
	for (UInt16 i = 0; i < i_valueS.GetSize(); i++)
		fields.Add(i, i_valueS[i]);
}

UInt32 ResultRow::operator[](const UInt16 i_index)
{
	if (!fields.ContainsKey(i_index))
		fields.Add(i_index, 0);
	return fields[i_index];
}

void ResultRow::Add(GS::Array<UInt32> i_valueS)
{
	for (UInt16 i = 0; i < i_valueS.GetSize(); i++)
		fields.Add(i, i_valueS[i]);
}

UInt16 ResultRow::Get(UInt16 i_col) const
{
	if (fields.ContainsKey(i_col))
	{
		return fields[i_col];
	}
	else
		throw 1;
}

ResultRow ResultRow::operator+ (const ResultRow& i_other)
{
	ResultRow result = ResultRow(*this);
	result.fields = fields;

	UInt32 l = fields.GetSize();

	for (auto k : i_other.fields.Keys())
		result.fields.Add(k + l, i_other.fields[k]);

	return result;
}

