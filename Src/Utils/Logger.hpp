#ifndef _LOGGER_HPP
#define _LOGGER_HPP

#include	"../APIEnvir.h"
#include	"ACAPinc.h"					// also includes APIdefs.h
#include	"../APICommon.h"
#include	"../Utils/Loglevels.hpp"
#include	"FileSystem.hpp"
#include	"File.hpp"


class Logevent 
{
	Loglevel		m_loglevel;
	GS::UniString	m_sLogText;
public:
	Logevent(GS::UniString i_sLogText, Loglevel i_loglevel) :
		m_sLogText	(i_sLogText),
		m_loglevel	(i_loglevel) {};
};


class Logger
{
public:
	enum NewLineFlag {
		NoNewLine,
		WithNewLine
	};
private:
	IO::Location*	m_logFileLocation;
	IO::File*		m_logFile;
	Loglevel		m_loglevel;

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
	void Log(Logevent i_logevent);
	void SetLoglevel(Loglevel i_loglevel) { m_loglevel = i_loglevel; };
	void SetLoglevel(short i_loglevel) { m_loglevel = (Loglevel)i_loglevel; };
	GS::UniString GetLogFileFolderStr();
	void SetLogFileFolder(IO::Location& i_loc);
	void SetLogFileFolder(IO::Location& i_loc, GS::UniString& i_fileName);
	void Logger::SetLogFileFolder(GS::UniString i_loc);
};

#endif //_LOGGER_HPP

