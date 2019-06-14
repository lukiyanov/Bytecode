#include "Exceptions.h"
using namespace interpreter;
using namespace std;

#ifdef ALLOW_CHECKS

const int bufsize = 32;
char buf[bufsize];
//------------------------------------------------------------------------------
string CInterpretException_UnknownCommand::Message()
{
	string str ("Unknown opcode found. Opcode: 0x");
	_ui64toa_s( m_opCode, buf, bufsize, 16 );
	str += buf;
	str += " at offset 0x";
	_ui64toa_s( m_rvaOffset, buf, bufsize, 16 );
	str += buf;

	return str;
}

//------------------------------------------------------------------------------
string CInterpretException_ReferenceIsOutOfAllocatedMemory::Message()
{
	string str ("Reference is out of an allocated memory at offset 0x");

	_ui64toa_s( m_rvaOffset, buf, bufsize, 16 );
	str += buf;
	str += ".\n";

	switch ( m_tReason )
	{
	case trRead:
		str += "The memory cannot be read.";
		break;

	case trWrite:
		str += "The memory cannot be wrote.";
		break;

	case trExecute:
		str += "The memory cannot be executed.";
		break;

	default:
		str += "Unknown reason.";
		break;
	}

	return str;
}

//------------------------------------------------------------------------------
string CInterpretException_StackIsEnded::Message()
{
	string str ("Call stack is ended.\nTry to allocate 0x");
	
	_ui64toa_s( m_wdCount, buf, bufsize, 16 );
	str += buf;

	str += " bytes with real stack size 0x";

	_ui64toa_s( m_dwStackSize, buf, bufsize, 16 );
	str += buf;

	str += " bytes.";

	return str;
}

//------------------------------------------------------------------------------
string CInterpretException_UnknownCallbackID::Message()
{
	string str ("Unknown callback-function ID.\nLast registered: 0x");
	
	_ui64toa_s( m_dwMaxID, buf, bufsize, 16 );
	str += buf;

	str += ". Found: 0x";

	_ui64toa_s( m_dwFoundID, buf, bufsize, 16 );
	str += buf;

	str += ".";

	return str;
}

//------------------------------------------------------------------------------
#endif