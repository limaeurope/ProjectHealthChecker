#include	"Utils.hpp"


// -----------------------------------------------------------------------------
// Load a localisable Unicode string from resource
// -----------------------------------------------------------------------------

extern void		GetStringFromResource(GS::UniString* buffer, short resID, short stringID)
{
	if (buffer != nullptr && !RSGetIndString(buffer, resID, stringID, ACAPI_GetOwnResModule()))
		buffer->Clear();

	return;
}

extern GS::UniString GetStringFromResource_(short resID, short stringID)
{
	GS::UniString *buffer = new GS::UniString;

	RSGetIndString(buffer, resID, stringID, ACAPI_GetOwnResModule());

	GS::UniString result = *buffer;
	auto _s = result.ToCStr().Get();

	return result;
}

extern GS::UniString GSFR(IntStr stringID)
{
	GS::UniString _r = GetStringFromResource_(TABLE_DATA, (short)stringID);
	auto _s = _r.ToCStr().Get();
	return _r;
}
// GetStringFromResource

