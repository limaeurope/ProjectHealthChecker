#ifndef _SETTINGSSINGLETON_HPP
#define _SETTINGSSINGLETON_HPP

#include	"DataStructs.hpp"
#include	<mutex>

class SettingsSingleton {
	SettingsSingleton();
	~SettingsSingleton();
	SettingsSingleton(const SettingsSingleton&);
	bool operator=(const SettingsSingleton&) {};
public:
	ResultTable	resultTable;
	CntlDlgData	cntlDlgData;

	static SettingsSingleton* GetInstance();
};

static SettingsSingleton* singleton = nullptr;
static std::mutex _mutex;


#endif // !_SETTINGSSINGLETON_HPP

