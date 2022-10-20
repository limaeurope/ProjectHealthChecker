#include "Logger.hpp"

#define		EOLInFile		"\xD\xA"
#define		EOLInFile_LEN	2


Logger::~Logger()
{
	CloseLogFile(); 
	delete m_logFileLocation;
}

void Logger::SetLogFileFolder(IO::Location& i_loc)
{ 
	m_logFileLocation = new IO::Location(i_loc);

	OpenLogFileForWriting();

	Write("Teszt2");

	Write("Teszt3");
}

void Logger::SetLogFileFolder(GS::UniString i_loc)
{ 
	return SetLogFileFolder(IO::Location(i_loc));
}

void Logger::SetLogFileFolder(IO::Location& i_loc, GS::UniString& i_fileName)
{ 
	//GSErrCode errCode = IO::fileSystem.CreateFolderTree(*m_logFileLocation);
	GSErrCode errCode = i_loc->GetStatus();

	if (errCode != NoError) {
		errCode = IO::fileSystem.CreateFolderTree(*m_logFileLocation);
	}

	return SetLogFileFolder(IO::Location(i_loc));
}

GS::UniString Logger::GetLogFileFolderStr()
{
	GS::UniString n;
	m_logFileLocation->ToPath(&n);
	auto _m = n.ToCStr();

	return (n);
}

void Logger::Log(Logevent i_logevent)
{

}

// =====================================================================================================================
//
// File I/O
//
// =====================================================================================================================

// -----------------------------------------------------------------------------
//	Write a unicode string into the open file
// -----------------------------------------------------------------------------

GSErrCode Logger::Write(const GS::UniString& val)
{
	GSErrCode lastErr = NoError;

	if (lastErr != NoError)
		return lastErr;

	Int32 length = Strlen32(val.ToCStr(0, MaxUSize, CC_UTF8).Get());
	char* cBuffer = nullptr;
	try {
		cBuffer = new char[length + 1];	// buffer_overrun_reviewed_0
		if (cBuffer != nullptr) {
			CHCopyC(val.ToCStr(0, MaxUSize, CC_UTF8).Get(), cBuffer);
			lastErr = m_logFile->WriteBin(cBuffer, length);
		}
		else {
			lastErr = ErrMemoryFull;
		}
	}
	catch (...) {
		lastErr = Error;
	}
	if (cBuffer != nullptr)
		delete[] cBuffer;

	return lastErr;
}		// Write

// -----------------------------------------------------------------------------
//	Write into the open file
// -----------------------------------------------------------------------------

GSErrCode Logger::Write(Int32 nBytes, GSPtr data)
{
	GSErrCode lastErr = NoError;

	if (lastErr != NoError)
		return lastErr;

	lastErr = m_logFile->WriteBin(data, nBytes);

	return lastErr;
}		// Write


// -----------------------------------------------------------------------------
//	Write a unicode string into the open file
// -----------------------------------------------------------------------------

GSErrCode Logger::AddToLogFile(const GS::UniString& i_logRow)
{
	GSErrCode lastErr = NoError;

	if (lastErr != NoError)
		return lastErr;

	Int32 length = Strlen32(i_logRow.ToCStr(0, MaxUSize, CC_UTF8).Get());
	char* cBuffer = nullptr;
	try {
		cBuffer = new char[length + 1];	// buffer_overrun_reviewed_0
		if (cBuffer != nullptr) {
			CHCopyC(i_logRow.ToCStr(0, MaxUSize, CC_UTF8).Get(), cBuffer);
			lastErr = m_logFile->WriteBin(cBuffer, length);
		}
		else {
			lastErr = ErrMemoryFull;
		}
	}
	catch (...) {
		lastErr = Error;
	}
	if (cBuffer != nullptr)
		delete[] cBuffer;

	return lastErr;
}		// Write


// -----------------------------------------------------------------------------
//	Open the file
// -----------------------------------------------------------------------------


GSErrCode Logger::OpenLogFileForWriting()
{
	GSErrCode	errCode = NoError;

	if (m_logFile != nullptr)
		return Error;

	IO::Location* fileLoc = m_logFileLocation;
	if (fileLoc == nullptr)
		return Error;

	//errCode = fileLoc->GetStatus();

	//if (errCode != NoError) {
	//	errCode = IO::fileSystem.CreateFolderTree(*m_logFileLocation);
	//}

	try {
		m_logFile = new IO::File(*fileLoc, IO::File::Create);
		if (m_logFile == nullptr)
			throw GS::GeneralException();
	}
	catch (...) {
		if (m_logFile != nullptr) {
			delete m_logFile;
			m_logFile = nullptr;
		}

		throw GS::GeneralException();
	}

	errCode = m_logFile->GetStatus();
	if (errCode == NoError)
		errCode = m_logFile->Open(IO::File::WriteEmptyMode);

	if (errCode != NoError) {
		m_logFile->Close();

		delete m_logFile;
		m_logFile = nullptr;
		IO::fileSystem.Delete(*fileLoc);

		throw GS::GeneralException();
	}

	if (m_logFile != nullptr)
		m_logFile->WriteBin("\xEF\xBB\xBF", 3);	// make it UTF-8

	return NoError;
}

// -----------------------------------------------------------------------------
//	Close the file
// -----------------------------------------------------------------------------


GSErrCode Logger::CloseLogFile(void)
{
	GSErrCode lastErr = NoError;

	if (m_logFile != nullptr)
		lastErr = m_logFile->Close();

	return lastErr;
}		// Close

// -----------------------------------------------------------------------------
//	Write a newline into the file
// -----------------------------------------------------------------------------


GSErrCode Logger::WrNewLine(void)
{
	GSErrCode lastErr = NoError;

	m_logFile->WriteBin(EOLInFile, EOLInFile_LEN);

	return lastErr;
}		// WrNewLine


GSErrCode	Logger::WriteStr(const char* val, NewLineFlag newLine /* = NoNewLine*/)
{
	GSErrCode lastErr = NoError;

	char	buffer[512];

	sprintf(buffer, "%s ", val);
	Write(Strlen32(buffer), buffer);

	if (newLine == WithNewLine)
		WrNewLine();

	return lastErr;
}		// WriteStr


// -----------------------------------------------------------------------------
//	Write a unicode string value into the file
// -----------------------------------------------------------------------------

GSErrCode	Logger::WriteStr(const GS::UniString& val, NewLineFlag newLine /* = NoNewLine*/)
{
	GSErrCode lastErr = NoError;

	Write(val + " ");

	if (newLine == WithNewLine)
		WrNewLine();

	return lastErr;
}		// WriteStr

