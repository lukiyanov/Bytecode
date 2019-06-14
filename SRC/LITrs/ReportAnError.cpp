#include "ReportAnError.h"
#include <stdlib.h>

void ReportAnError( const std::string& str, const int line )
{
	const int bufsize = 64;
	static char buf[ bufsize ];

	std::string strFull = str + " at line: ";
	_itoa_s( line, buf, bufsize, 10 );
	strFull += buf;
	throw std::exception( strFull.c_str() );
}