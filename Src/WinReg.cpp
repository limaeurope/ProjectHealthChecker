#include	"WinReg.hpp"

GS::UniString GetRegString(GS::UniString i_key, GS::UniString i_path, HKEY i_hKey/* = HKEY_CURRENT_USER*/)
{
	char sBuffer[255];
	DWORD iBuffer = 255;

	LSTATUS _ = RegGetValueW(i_hKey,
		i_path.ToUStr().Get(),		/*L"SOFTWARE\\GRAPHISOFT\\ARCHICAD\\Archicad 26.0.0 INT R1\\Add-On Manager",*/
		i_key.ToUStr().Get(),		/*L"Default Location",*/
		RRF_RT_REG_SZ,
		NULL,
		&sBuffer,
		&iBuffer);

	wchar_t* ws = (wchar_t*)sBuffer;

	return GS::UniString(ws);
}

UInt32 GetRegInt(GS::UniString i_key, GS::UniString i_path, HKEY i_hKey/* = HKEY_CURRENT_USER*/)
{
	DWORD iBuffer = 0;
	DWORD iBufferSize = 255;

	LSTATUS _ = RegGetValueW(i_hKey,
		i_path.ToUStr().Get(),		/*L"SOFTWARE\\GRAPHISOFT\\ARCHICAD\\Archicad 26.0.0 INT R1\\Add-On Manager",*/
		i_key.ToUStr().Get(),		/*L"Use Default Location",*/
		RRF_RT_REG_DWORD,
		NULL,
		&iBuffer,
		&iBufferSize);

	return UInt16(iBuffer);
}

HKEY GetOrCreateRegPath(GS::UniString i_path, HKEY i_hKey/* = HKEY_CURRENT_USER*/)
{
	HKEY hKey;
	LSTATUS status = RegCreateKeyExW(i_hKey,
		(wchar_t*)(i_path.ToUStr().Get()),
		0,
		NULL,
		REG_OPTION_NON_VOLATILE,
		KEY_WRITE,
		NULL,
		&hKey,
		NULL
	);

	return hKey;
}

void SetRegString(GS::UniString i_val, GS::UniString i_key, GS::UniString i_path, HKEY i_hKey/* = HKEY_CURRENT_USER*/)
{
	HKEY hKey = GetOrCreateRegPath(i_path, i_hKey);

	auto wVal = (wchar_t*)(i_val.ToUStr().Get());
	auto key = (wchar_t*)(i_key.ToUStr().Get());

	auto _b = (BYTE*)wVal;
	auto _bs = (DWORD)(i_val.GetLength()*2);

	LSTATUS status = RegSetValueExW(hKey,
		key,
		0,
		REG_SZ,
		_b,
		_bs);

	status = RegCloseKey(i_hKey);
	status = RegCloseKey(hKey);
}

void SetRegInt(UInt32 i_val, GS::UniString i_key, GS::UniString i_path, HKEY i_hKey/* = HKEY_CURRENT_USER*/)
{
	HKEY hKey = GetOrCreateRegPath(i_path, i_hKey);

	auto skey = (wchar_t*)(i_path.ToUStr().Get());
	auto key = (wchar_t*)(i_key.ToUStr().Get());

	auto _b = (const BYTE*)&i_val;
	auto _bs = (DWORD)sizeof(i_val);

	LSTATUS status = RegSetValueExW(hKey,
		key,
		0,
		REG_DWORD,
		_b,
		_bs);

	status = RegCloseKey(i_hKey);
	status = RegCloseKey(hKey);
}

