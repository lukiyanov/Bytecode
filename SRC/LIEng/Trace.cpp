#include "Trace.h"
using namespace std;

//------------------------------------------------------------------------------
bool bTraceEnabled = 0;

//------------------------------------------------------------------------------
void Trace(const std::string& str, ostream& out)
{
	if (bTraceEnabled)
	{
		out << str << endl;
	}
}

//------------------------------------------------------------------------------
void EnableTrace(const bool bEnable)
{
	bTraceEnabled = bEnable;
}

//------------------------------------------------------------------------------
bool GetTraceEnabled()
{
	return bTraceEnabled;
}

//------------------------------------------------------------------------------