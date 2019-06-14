#include "..\LIEng\COP.h"
using namespace interpreter;

#include <string>
#include <fstream>
using namespace std;

////////////////////////////////////////////////////////////////////////////////
// Определения, используемые в байт-коде
#define jmp		copJMP32v, 0x00,
#define halt	copHALT,   0x00,
#define xlock	copXLOCKm, 0x00,
#define xlock_0	copXLOCK,	0x00,
#define push_v	copPUSH32v, 0x00,
#define push_m	copPUSH32m, 0x00,
#define lea_f	copLEA32mfv, 0x00,
#define xcall	copXCALL32v, 0x00,
#define mov_v	copMOV32mv, 0x00,
#define mov_m	copMOV32mm, 0x00,
#define cmp_e	copCMPE32mv, 0x00,
#define jct		copJCT32v, 0x00,
#define jcf		copJCF32v, 0x00,
#define pop		copPOP32m, 0x00,
#define xret	copXRET32m, 0x00,

#define lea		copLEA32mv, 0x00,
#define movp	copMOV16ppoo, 0x00,
#define add		copADD16mm, 0x00,
#define idiv	copIDIV16mm, 0x00,

#define bto_dword(value) value, 0x00, 0x00, 0x00,
#define entry bto_dword(0x12)

#define var1 bto_dword(0x06)
#define var2 bto_dword(0x08)
#define ptr1  bto_dword(0x0a)
#define ptr2  bto_dword(0x0e)

////////////////////////////////////////////////////////////////////////////////
// Байт-код
unsigned char aCode[] = {
	jmp entry

	0xF4, 0xFF,						// var1
	0xFD, 0xFF,						// var2
	0x00, 0x00,	0x00, 0x00,			// ptr1
	0x00, 0x00,	0x00, 0x00,			// ptr2

	idiv  var1 var2
	halt
};

const int nCodeSize = sizeof( aCode );

////////////////////////////////////////////////////////////////////////////////

int main()
{
	ofstream fout( "program.bytecode", ios::out | ios::binary );
	fout.write( (char*)aCode, nCodeSize );
	fout.flush();
	fout.close();

	fout.open( "program.extern", ios::out );
	fout << 0 << ' ' << 0 << endl;
	fout.flush();
	fout.close();
}

////////////////////////////////////////////////////////////////////////////////
