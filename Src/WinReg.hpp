#ifndef _WINREG_HPP
#define _WINREG_HPP

#include	"APIEnvir.h"
#include	"ACAPinc.h"					// also includes APIdefs.h
#include	"APICommon.h"

#include	"winreg.h"

const GS::UniString REG_PATH("SOFTWARE\\LIMA\\ProjectHealthChecker");
const HKEY _HKEY = HKEY_CURRENT_USER;

GS::UniString GetRegString(GS::UniString i_key, GS::UniString i_path = REG_PATH, HKEY i_hkey = _HKEY);

inline GS::UniString GetRegString(char* i_key, char* i_path, HKEY i_hkey = _HKEY)
	{ return GetRegString(GS::UniString(i_key), GS::UniString(i_path), i_hkey); }
inline GS::UniString GetRegString(wchar_t* i_key, wchar_t* i_path, HKEY i_hkey = _HKEY)
	{return GetRegString(GS::UniString(i_key), GS::UniString(i_path), i_hkey);}

UInt32 GetRegInt(GS::UniString i_key, GS::UniString i_path = REG_PATH, HKEY i_hkey = _HKEY);

inline UInt32 GetRegInt(char* i_key, char* i_path, HKEY i_hkey = _HKEY)
	{return GetRegInt(GS::UniString(i_key), GS::UniString(i_path), i_hkey);}
inline UInt32 GetRegInt(wchar_t* i_key, wchar_t* i_path, HKEY i_hkey = _HKEY)
	{return GetRegInt(GS::UniString(i_key), GS::UniString(i_path), i_hkey);}

void SetRegString(GS::UniString i_val, GS::UniString i_key, GS::UniString i_path = REG_PATH, HKEY i_hkey = _HKEY);

inline void SetRegString(GS::UniString i_val, char* i_key, char* i_path, HKEY i_hkey = _HKEY)
	{return SetRegString(i_val, GS::UniString(i_key), GS::UniString(i_path), i_hkey); }
inline void SetRegString(GS::UniString i_val, wchar_t* i_key, wchar_t* i_path, HKEY i_hkey = _HKEY)
	{return SetRegString(i_val, GS::UniString(i_key), GS::UniString(i_path), i_hkey); }

void SetRegInt(UInt32 i_val, GS::UniString i_key, GS::UniString i_path = REG_PATH, HKEY i_hkey = _HKEY);

inline void SetRegInt(UInt32 i_val, char* i_key, char* i_path, HKEY i_hkey = _HKEY)
	{return SetRegInt(i_val, GS::UniString(i_key), GS::UniString(i_path), i_hkey);}
inline void SetRegInt(UInt32 i_val, wchar_t* i_key, wchar_t* i_path, HKEY i_hkey = _HKEY)
	{return SetRegInt(i_val, GS::UniString(i_key), GS::UniString(i_path), i_hkey); }

#endif //_WINREG_HPP

