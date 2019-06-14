#include <typeinfo.h>
#include "TranslationSchemeTable.h"
#include "..\LIEng\COP.h"

////////////////////////////////////////////////////////////////////////////////
#define ARGSET_1( b, t)\
CArgumentParameters args_##b##t[] =\
{\
	{ b, t },\
	{ 0, 0 }\
};

#define ARGSET_2( b1, t1, b2, t2)\
CArgumentParameters args_##b1##t1##_##b2##t2[] =\
{\
	{ b1, t1 },\
	{ b2, t2 },\
	{ 0,  0 }\
};

////////////////////////////////////////////////////////////////////////////////

const RTTI m = RUNTIME_CLASS( CVariableArg );
const RTTI v = RUNTIME_CLASS( CValueArg    );
const RTTI p = RUNTIME_CLASS( CPointerArg  );

////////////////////////////////////////////////////////////////////////////////
// ARGSETS
CArgumentParameters args_none[] =
{ { 0, 0 } };

ARGSET_1( 8,  v )
ARGSET_1( 8,  m )
ARGSET_1( 16, v )
ARGSET_1( 16, m )
ARGSET_1( 32, v )
ARGSET_1( 32, m )

ARGSET_2( 8,  m, 8,  v )
ARGSET_2( 8,  v, 8,  m )
ARGSET_2( 8,  m, 8,  m )
ARGSET_2( 16, m, 16, v )
ARGSET_2( 16, v, 16, m )
ARGSET_2( 16, m, 16, m )
ARGSET_2( 32, m, 32, v )
ARGSET_2( 32, v, 32, m )
ARGSET_2( 32, m, 32, m )

ARGSET_2( 8,  m, 8,  p )
ARGSET_2( 16, m, 16, p )
ARGSET_2( 32, m, 32, p )
ARGSET_2( 8,  p, 8,  m )
ARGSET_2( 16, p, 16, m )
ARGSET_2( 32, p, 32, m )
ARGSET_2( 8,  p, 8,  v )
ARGSET_2( 16, p, 16, v )
ARGSET_2( 32, p, 32, v )
ARGSET_2( 8,  p, 8,  p )
ARGSET_2( 16, p, 16, p )
ARGSET_2( 32, p, 32, p )

ARGSET_2( 16, m, 8, m )
ARGSET_2( 32, m, 8, m )
ARGSET_2( 32, m, 16, m )

////////////////////////////////////////////////////////////////////////////////
// NOP
CTranslationScheme tsNOP[] =
{
	{ copNOP, args_none },

	{ copLastCOP, 0 }
};

// HALT
CTranslationScheme tsHALT[] =
{
	{ copHALT, args_none },

	{ copLastCOP, 0 }
};

// MOV, MOVP
CTranslationScheme tsMOV[] =
{
	{ copMOV8mm,  args_8m_8m   },
	{ copMOV8mv,  args_8m_8v   },
	{ copMOV16mm, args_16m_16m },
	{ copMOV16mv, args_16m_16v },
	{ copMOV32mm, args_32m_32m },
	{ copMOV32mv, args_32m_32v },

	// MOV-P:
	{ copMOV8mpo,   args_8m_8p   },
	{ copMOV16mpo,  args_16m_16p },
	{ copMOV32mpo,  args_32m_32p },

	{ copMOV8pom,   args_8p_8m   },
	{ copMOV16pom,  args_16p_16m },
	{ copMOV32pom,  args_32p_32m },

	{ copMOV8pov,   args_8p_8v   },
	{ copMOV16pov,  args_16p_16v },
	{ copMOV32pov,  args_32p_32v },

	{ copMOV8popo,  args_8p_8p   },
	{ copMOV16popo, args_16p_16p },
	{ copMOV32popo, args_32p_32p },

	{ copLastCOP, 0 }
};

//------------------------------------------------------------------------------
// MOVXX:
//------------------------------------------------------------------------------
// MOVSX
CTranslationScheme tsMOVSX[] =
{
	{ copMOVSX16x8mm,  args_16m_8m  },
	{ copMOVSX32x8mm,  args_32m_8m  },
	{ copMOVSX32x16mm, args_32m_16m },

	{ copLastCOP, 0 }
};

// MOVZX
CTranslationScheme tsMOVZX[] =
{
	{ copMOVZX16x8mm,  args_16m_8m  },
	{ copMOVZX32x8mm,  args_32m_8m  },
	{ copMOVZX32x16mm, args_32m_16m },

	{ copLastCOP, 0 }
};

//------------------------------------------------------------------------------
// JMP:
//------------------------------------------------------------------------------
CTranslationScheme tsJMP[] =
{
	{ copJMP32m, args_32m },
	{ copJMP32v, args_32v },

	{ copLastCOP, 0 }
};

//------------------------------------------------------------------------------
// Работа со стеком:
//------------------------------------------------------------------------------
// PUSH
CTranslationScheme tsPUSH[] =
{
	{ copPUSH8v,  args_8v  },
	{ copPUSH16v, args_16v },
	{ copPUSH32v, args_32v },
	{ copPUSH8m,  args_8m  },
	{ copPUSH16m, args_16m },
	{ copPUSH32m, args_32m },

	{ copLastCOP, 0 }
};

// POP
CTranslationScheme tsPOP[] =
{
	{ copPOP8m,  args_8m  },
	{ copPOP16m, args_16m },
	{ copPOP32m, args_32m },

	{ copLastCOP, 0 }
};

// SPOP
CTranslationScheme tsSPOP[] =
{
	{ copSPOP,    args_none },

	{ copSPOP16v, args_16v  },
	{ copSPOP16m, args_16m  },

	{ copLastCOP, 0 }
};

// LOCK
CTranslationScheme tsLOCK[] =
{
	{ copLOCK,  args_none },

	{ copLOCKm, args_8m   },
	{ copLOCKm, args_16m  },
	{ copLOCKm, args_32m  },

	{ copLastCOP, 0 }
};

// CALL
CTranslationScheme tsCALL[] =
{
	{ copCALL32v, args_32v },
	{ copCALL32m, args_32m },

	{ copLastCOP, 0 }
};

// RET
CTranslationScheme tsRET[] =
{
	{ copRET, args_none },

	{ copRET8v,  args_8v  },
	{ copRET8m,  args_8m  },

	{ copRET16v, args_16v },
	{ copRET16m, args_16m },

	{ copRET32v, args_32v },
	{ copRET32m, args_32m },

	{ copLastCOP, 0 }
};

//------------------------------------------------------------------------------
// Условные переходы (JCx) и сравнения (CMPxx):
//------------------------------------------------------------------------------
// CMPE
CTranslationScheme tsCMPE[] =
{
	{ copCMPE8mm,  args_8m_8m },
	{ copCMPE8mv,  args_8m_8v },
	{ copCMPE8vm,  args_8v_8m },
	{ copCMPE16mm, args_16m_16m },
	{ copCMPE16mv, args_16m_16v },
	{ copCMPE16vm, args_16v_16m },
	{ copCMPE32mm, args_32m_32m },
	{ copCMPE32mv, args_32m_32v },
	{ copCMPE32vm, args_32v_32m },

	{ copLastCOP, 0 }
};

// CMPNE
CTranslationScheme tsCMPNE[] =
{
	{ copCMPNE8mm,  args_8m_8m },
	{ copCMPNE8mv,  args_8m_8v },
	{ copCMPNE8vm,  args_8v_8m },
	{ copCMPNE16mm, args_16m_16m },
	{ copCMPNE16mv, args_16m_16v },
	{ copCMPNE16vm, args_16v_16m },
	{ copCMPNE32mm, args_32m_32m },
	{ copCMPNE32mv, args_32m_32v },
	{ copCMPNE32vm, args_32v_32m },

	{ copLastCOP, 0 }
};

// CMPB
CTranslationScheme tsCMPB[] =
{
	{ copCMPB8mm,  args_8m_8m },
	{ copCMPB8mv,  args_8m_8v },
	{ copCMPB8vm,  args_8v_8m },
	{ copCMPB16mm, args_16m_16m },
	{ copCMPB16mv, args_16m_16v },
	{ copCMPB16vm, args_16v_16m },
	{ copCMPB32mm, args_32m_32m },
	{ copCMPB32mv, args_32m_32v },
	{ copCMPB32vm, args_32v_32m },

	{ copLastCOP, 0 }
};

// CMPBE
CTranslationScheme tsCMPBE[] =
{
	{ copCMPBE8mm,  args_8m_8m },
	{ copCMPBE8mv,  args_8m_8v },
	{ copCMPBE8vm,  args_8v_8m },
	{ copCMPBE16mm, args_16m_16m },
	{ copCMPBE16mv, args_16m_16v },
	{ copCMPBE16vm, args_16v_16m },
	{ copCMPBE32mm, args_32m_32m },
	{ copCMPBE32mv, args_32m_32v },
	{ copCMPBE32vm, args_32v_32m },

	{ copLastCOP, 0 }
};

// CMPL
CTranslationScheme tsCMPL[] =
{
	{ copCMPL8mm,  args_8m_8m },
	{ copCMPL8mv,  args_8m_8v },
	{ copCMPL8vm,  args_8v_8m },
	{ copCMPL16mm, args_16m_16m },
	{ copCMPL16mv, args_16m_16v },
	{ copCMPL16vm, args_16v_16m },
	{ copCMPL32mm, args_32m_32m },
	{ copCMPL32mv, args_32m_32v },
	{ copCMPL32vm, args_32v_32m },

	{ copLastCOP, 0 }
};

// CMPLE
CTranslationScheme tsCMPLE[] =
{
	{ copCMPLE8mm,  args_8m_8m },
	{ copCMPLE8mv,  args_8m_8v },
	{ copCMPLE8vm,  args_8v_8m },
	{ copCMPLE16mm, args_16m_16m },
	{ copCMPLE16mv, args_16m_16v },
	{ copCMPLE16vm, args_16v_16m },
	{ copCMPLE32mm, args_32m_32m },
	{ copCMPLE32mv, args_32m_32v },
	{ copCMPLE32vm, args_32v_32m },

	{ copLastCOP, 0 }
};

// JCT
CTranslationScheme tsJCT[] =
{
	{ copJCT32v, args_32v },
	{ copJCT32m, args_32m },

	{ copLastCOP, 0 }
};

// JCF
CTranslationScheme tsJCF[] =
{
	{ copJCF32v, args_32v },
	{ copJCF32m, args_32m },

	{ copLastCOP, 0 }
};

//------------------------------------------------------------------------------
// Внешние вызовы:
//------------------------------------------------------------------------------
// XLOCK
CTranslationScheme tsXLOCK[] =
{
	{ copXLOCK,  args_none },

	{ copXLOCKm, args_8m  },
	{ copXLOCKm, args_16m },
	{ copXLOCKm, args_32m },

	{ copLastCOP, 0 }
};

// XCALL
CTranslationScheme tsXCALL[] =
{
	{ copXCALL32v, args_32v },
	{ copXCALL32m, args_32m },

	{ copLastCOP, 0 }
};

// XRET
CTranslationScheme tsXRET[] =
{
	{ copXRET,   args_none },

	{ copXRET8v,  args_8v  },
	{ copXRET8m,  args_8m  },
	{ copXRET16v, args_16v },
	{ copXRET16m, args_16m },
	{ copXRET32v, args_32v },
	{ copXRET32m, args_32m },

	{ copLastCOP, 0 }
};

// LEA
CTranslationScheme tsLEA[] =
{
	{ copLEA32mv,  args_32m_8m },
	{ copLEA32mv,  args_32m_16m },
	{ copLEA32mv,  args_32m_32m },

	{ copLastCOP, 0 }
};

CTranslationScheme tsLEAF[] =
{
	{ copLEA32mfv, args_32m_32v },
	{ copLEA32mfm, args_32m_32m },

	{ copLastCOP, 0 }
};

//------------------------------------------------------------------------------
// Арифметические команды
//------------------------------------------------------------------------------

// ADD
CTranslationScheme tsADD[] =
{
	{ copADD8mm,  args_8m_8m },
	{ copADD8mv,  args_8m_8v },
	{ copADD16mm, args_16m_16m },
	{ copADD16mv, args_16m_16v },
	{ copADD32mm, args_32m_32m },
	{ copADD32mv, args_32m_32v },

	{ copLastCOP, 0 }
};

// SUB
CTranslationScheme tsSUB[] =
{
	{ copSUB8mm,  args_8m_8m },
	{ copSUB8mv,  args_8m_8v },
	{ copSUB16mm, args_16m_16m },
	{ copSUB16mv, args_16m_16v },
	{ copSUB32mm, args_32m_32m },
	{ copSUB32mv, args_32m_32v },

	{ copLastCOP, 0 }
};

// MUL
CTranslationScheme tsMUL[] =
{
	{ copMUL8mm,  args_8m_8m },
	{ copMUL8mv,  args_8m_8v },
	{ copMUL16mm, args_16m_16m },
	{ copMUL16mv, args_16m_16v },
	{ copMUL32mm, args_32m_32m },
	{ copMUL32mv, args_32m_32v },

	{ copLastCOP, 0 }
};

// IMUL
CTranslationScheme tsIMUL[] =
{
	{ copIMUL8mm,  args_8m_8m },
	{ copIMUL8mv,  args_8m_8v },
	{ copIMUL16mm, args_16m_16m },
	{ copIMUL16mv, args_16m_16v },
	{ copIMUL32mm, args_32m_32m },
	{ copIMUL32mv, args_32m_32v },

	{ copLastCOP, 0 }
};

// DIV
CTranslationScheme tsDIV[] =
{
	{ copDIV8mm,  args_8m_8m },
	{ copDIV8mv,  args_8m_8v },
	{ copDIV16mm, args_16m_16m },
	{ copDIV16mv, args_16m_16v },
	{ copDIV32mm, args_32m_32m },
	{ copDIV32mv, args_32m_32v },

	{ copLastCOP, 0 }
};

// IDIV
CTranslationScheme tsIDIV[] =
{
	{ copIDIV8mm,  args_8m_8m },
	{ copIDIV8mv,  args_8m_8v },
	{ copIDIV16mm, args_16m_16m },
	{ copIDIV16mv, args_16m_16v },
	{ copIDIV32mm, args_32m_32m },
	{ copIDIV32mv, args_32m_32v },

	{ copLastCOP, 0 }
};

// NEG
CTranslationScheme tsNEG[] =
{
	{ copNEG8m,  args_8m  },
	{ copNEG16m, args_16m },
	{ copNEG32m, args_32m },

	{ copLastCOP, 0 }
};

//------------------------------------------------------------------------------
// Поразрядные команды
//------------------------------------------------------------------------------

// AND
CTranslationScheme tsAND[] =
{
	{ copAND8mm,  args_8m_8m },
	{ copAND8mv,  args_8m_8v },
	{ copAND16mm, args_16m_16m },
	{ copAND16mv, args_16m_16v },
	{ copAND32mm, args_32m_32m },
	{ copAND32mv, args_32m_32v },

	{ copLastCOP, 0 }
};

// OR
CTranslationScheme tsOR[] =
{
	{ copOR8mm,  args_8m_8m },
	{ copOR8mv,  args_8m_8v },
	{ copOR16mm, args_16m_16m },
	{ copOR16mv, args_16m_16v },
	{ copOR32mm, args_32m_32m },
	{ copOR32mv, args_32m_32v },

	{ copLastCOP, 0 }
};

// XOR
CTranslationScheme tsXOR[] =
{
	{ copXOR8mm,  args_8m_8m },
	{ copXOR8mv,  args_8m_8v },
	{ copXOR16mm, args_16m_16m },
	{ copXOR16mv, args_16m_16v },
	{ copXOR32mm, args_32m_32m },
	{ copXOR32mv, args_32m_32v },

	{ copLastCOP, 0 }
};

// NOT
CTranslationScheme tsNOT[] =
{
	{ copNOT8m,  args_8m  },
	{ copNOT16m, args_16m },
	{ copNOT32m, args_32m },

	{ copLastCOP, 0 }
};

// template
CTranslationScheme tsTODO[] =
{
	{ copLastCOP, 0 }
};

////////////////////////////////////////////////////////////////////////////////
CTranslationSchemeTable::CTranslationSchemeTable()
{
	mapSchemes["nop" ]	= tsNOP;
	mapSchemes["halt"]	= tsHALT;

	mapSchemes["mov"  ]	= tsMOV;
	mapSchemes["movsx"]	= tsMOVSX;
	mapSchemes["movzx"]	= tsMOVZX;

	mapSchemes["jmp"]	= tsJMP;

	mapSchemes["push"]	= tsPUSH;
	mapSchemes["pop" ]	= tsPOP;
	mapSchemes["spop"]	= tsSPOP;
	mapSchemes["call"]	= tsCALL;
	mapSchemes["lock"]	= tsLOCK;
	mapSchemes["ret" ]	= tsRET;

	mapSchemes["cmpe" ]	= tsCMPE;
	mapSchemes["cmpne"]	= tsCMPNE;
	mapSchemes["cmpb" ]	= tsCMPB;
	mapSchemes["cmpbe"]	= tsCMPBE;
	mapSchemes["cmpl" ]	= tsCMPL;
	mapSchemes["cmple"]	= tsCMPLE;

	mapSchemes["jct"]	= tsJCT;
	mapSchemes["jcf"]	= tsJCF;

	mapSchemes["xcall"]	= tsXCALL;
	mapSchemes["xlock"]	= tsXLOCK;
	mapSchemes["xret" ]	= tsXRET;
	mapSchemes["lea"  ]	= tsLEA;
	mapSchemes["leaf" ]	= tsLEAF;

	mapSchemes["add" ]	= tsADD;
	mapSchemes["sub" ]	= tsSUB;
	mapSchemes["mul" ]	= tsMUL;
	mapSchemes["imul"]	= tsIMUL;
	mapSchemes["div" ]	= tsDIV;
	mapSchemes["idiv"]	= tsIDIV;
	mapSchemes["neg" ]	= tsNEG;

	mapSchemes["and"]	= tsAND;
	mapSchemes["or" ]	= tsOR;
	mapSchemes["xor"]	= tsXOR;
	mapSchemes["not"]	= tsNOT;
}

////////////////////////////////////////////////////////////////////////////////
bool CTranslationSchemeTable::CreateCommand(const CLexLine& line, /*OUT: */ CCommand& cmd, std::map< std::string, CReference* >& mapRef)
{
	schmap::iterator pos = mapSchemes.find( line.vct[0].strSource );

	// Такой команды не найдено
	if ( pos == mapSchemes.end() )
		return 0;

	// Строка, на которой мы находимся
	cmd.nLine = line.nLineNumber;

	// Получаем указатель на начало массива допустимых комбинаций аргументов
	// и соответствующим им COP'ам
	cmd.pTSArray = (*pos).second;

	// Произведем разбор аргументов
	cmd.vctArguments.clear();
	for (int pos = 1, size = line.vct.size(); pos < size; )
	{
		// Выбирем подстроку аргуметна
		CLexLine arg;
		arg.nLineNumber = line.nLineNumber;
		while ( pos < size )
		{
			if ( line.vct[ pos ].strSource == "," ) 
			{
				pos++;
				break;
			}

			arg.vct.push_back( line.vct[ pos++ ] );
		}

		CArgument *pArg = ConstructArg( arg, mapRef );

		if ( !pArg )
			return 0;

		cmd.vctArguments.push_back( pArg );
	}

	return 1;
}

////////////////////////////////////////////////////////////////////////////////
