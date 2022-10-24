#include	"DateTime.hpp"
#include	<ctime>


GS::UniString GetTimeStr(char * const i_formatCode /*= "%Y%m%d%H%M%S"*/)
{
	time_t now = time(0);
	tm* ltm = localtime(&now);
	
	char buffer[256];

	strftime(buffer, sizeof(buffer), i_formatCode, ltm);

	return GS::UniString(buffer);
}
