#ifndef _WINREG_HPP
#define _WINREG_HPP

#include	"APIEnvir.h"
#include	"ACAPinc.h"					// also includes APIdefs.h
#include	"APICommon.h"

#include	"winreg.h"

const char  REG_PATH[] = "SOFTWARE\\LIMA\\ProjectHealthChecker";
const HKEY _HKEY = HKEY_CURRENT_USER;

GS::UniString GetRegString(const GS::UniString i_key, const GS::UniString i_path = GS::UniString(REG_PATH), HKEY i_hkey = _HKEY);

inline GS::UniString GetRegString(const char* i_key, const char* i_path = REG_PATH, HKEY i_hkey = _HKEY)
	{ return GetRegString(GS::UniString(i_key), GS::UniString(i_path), i_hkey); }
inline GS::UniString GetRegString(const wchar_t* i_key, const wchar_t* i_path, HKEY i_hkey = _HKEY)
	{return GetRegString(GS::UniString(i_key), GS::UniString(i_path), i_hkey);}

UInt32 GetRegInt(const GS::UniString i_key, const GS::UniString i_path = GS::UniString(REG_PATH), HKEY i_hkey = _HKEY);

inline UInt32 GetRegInt(const char* i_key, const char* i_path = REG_PATH, HKEY i_hkey = _HKEY)
	{return GetRegInt(GS::UniString(i_key), GS::UniString(i_path), i_hkey);}
inline UInt32 GetRegInt(const wchar_t* i_key, const wchar_t* i_path, HKEY i_hkey = _HKEY)
	{return GetRegInt(GS::UniString(i_key), GS::UniString(i_path), i_hkey);}

void SetRegString(const GS::UniString i_val, const GS::UniString i_key, const GS::UniString i_path = GS::UniString(REG_PATH), HKEY i_hkey = _HKEY);

inline void SetRegString(const GS::UniString i_val, const char* i_key, const char* i_path = REG_PATH, HKEY i_hkey = _HKEY)
	{return SetRegString(i_val, GS::UniString(i_key), GS::UniString(i_path), i_hkey); }
inline void SetRegString(const GS::UniString i_val, const wchar_t* i_key, const wchar_t* i_path, HKEY i_hkey = _HKEY)
	{return SetRegString(i_val, GS::UniString(i_key), GS::UniString(i_path), i_hkey); }

void SetRegInt(const UInt32 i_val, const GS::UniString i_key, const GS::UniString i_path = REG_PATH, HKEY i_hkey = _HKEY);

inline void SetRegInt(const UInt32 i_val, const char* i_key, const char* i_path = REG_PATH, HKEY i_hkey = _HKEY)
	{return SetRegInt(i_val, GS::UniString(i_key), GS::UniString(i_path), i_hkey);}
inline void SetRegInt(const UInt32 i_val, const wchar_t* i_key, const wchar_t* i_path, HKEY i_hkey = _HKEY)
	{return SetRegInt(i_val, GS::UniString(i_key), GS::UniString(i_path), i_hkey); }

#endif //_WINREG_HPP

