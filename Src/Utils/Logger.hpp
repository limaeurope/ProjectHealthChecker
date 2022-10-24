#ifndef _LOGGER_HPP
#define _LOGGER_HPP

#include	"../APIEnvir.h"
#include	"ACAPinc.h"					// also includes APIdefs.h
#include	"../APICommon.h"
#include	"../Enums/Loglevels.hpp"
#include	"../Utils/DateTime.hpp"
#include	"FileSystem.hpp"
#include	"File.hpp"
#include	<mutex>

// TODO To be removed
class Logevent 
{
	Loglevel		m_loglevel;
	GS::UniString	m_sLogText;
	GS::UniString	m_sDate;
public:
	Logevent(GS::UniString i_sLogText, Loglevel i_loglevel) :
		m_sLogText	(i_sLogText),
		m_loglevel	(i_loglevel),
		m_sDate		(GetTimeStr()) {};
	const GS::UniString ToUniString() const;
	Loglevel GetLogLevel() const { return m_loglevel; }
};


class Logger
{
public:
	enum NewLineFlag {
		NoNewLine,
		WithNewLine
	};
private:
	IO::Location*		m_pLogFileFolder;
	GS::UniString		m_logFileName;
	mutable IO::File*	m_pLogFile;
	Loglevel			m_loglevel;

	GSErrCode OpenLogFileForWriting();
	GSErrCode CloseLogFile();

	GSErrCode AddToLogFile(const GS::UniString& i_logRow);
	GSErrCode WrNewLine(void);
	GSErrCode Write(Int32 nBytes, GSPtr data);
	GSErrCode Write(const GS::UniString& val);
	GSErrCode WriteStr(const char* val, NewLineFlag newLine /* = NoNewLine*/);
	GSErrCode WriteStr(const GS::UniString& val, NewLineFlag newLine /* = NoNewLine*/);
public:
	~Logger();
	Logger();
	void Log(const Logevent& i_logevent);
	void Log(const GS::UniString& i_sLogText, const GSErrCode i_errCode, const Loglevel i_logLevel = LogLev_DEBUG);
	inline void SetLoglevel(Loglevel i_loglevel) { m_loglevel = i_loglevel; };
	inline void SetLoglevel(short i_loglevel) { m_loglevel = (Loglevel)i_loglevel; };
	inline short GetLoglevel() { return (short) m_loglevel ; };
	GS::UniString GetLogFileFolderStr();
	void SetLogFileFolder(IO::Location& i_loc, GS::UniString& i_fileName);
	static Logger& GetLogger();
};

static std::mutex _loggerMutex;
extern Logger& (*LOGGER)();

#endif //_LOGGER_HPP

