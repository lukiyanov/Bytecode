#include "COP.h"
#include "Command.h"

#include "generic.h"
#include "mov.h"
#include "jmp.h"
#include "movxx.h"
#include "stack.h"
#include "condition.h"
#include "extern.h"

#include "movp.h"
#include "arithmetics.h"
#include "logical.h"

namespace interpreter {

	//------------------------------------------------------------------------------
	// Список доступных команд. Их порядок не обязательно должен совпадать с
	// объявлением именованных констант в ECOP.
	const CCommand aRegisteredCommands[] = {

		// Generic
		CCommand( copNOP,		&nop  ),
		CCommand( copBRK,		&brk  ),
		CCommand( copHALT,		&halt ),

		// MOV
		CCommand( copMOV8mm,	&mov8mm  ),
		CCommand( copMOV8mv,	&mov8mv  ),
		CCommand( copMOV16mm,	&mov16mm ),
		CCommand( copMOV16mv,	&mov16mv ),
		CCommand( copMOV32mm,	&mov32mm ),
		CCommand( copMOV32mv,	&mov32mv ),

		// JMP
		CCommand( copJMP32m,	&jmp32m ),
		CCommand( copJMP32v,	&jmp32v ),

		// MOVXX
		CCommand( copMOVSX16x8mm,  &movsx16x8mm  ),
		CCommand( copMOVSX32x8mm,  &movsx32x8mm  ),
		CCommand( copMOVSX32x16mm, &movsx32x16mm ),

		CCommand( copMOVZX16x8mm,  &movzx16x8mm  ),
		CCommand( copMOVZX32x8mm,  &movzx32x8mm  ),
		CCommand( copMOVZX32x16mm, &movzx32x16mm ),

		// -----------------------------------------------
		// Стек
		// -----------------------------------------------

		// PUSH
		CCommand( copPUSH8v,  push8v  ),
		CCommand( copPUSH16v, push16v ),
		CCommand( copPUSH32v, push32v ),
		CCommand( copPUSH8m,  push8m  ),
		CCommand( copPUSH16m, push16m ),
		CCommand( copPUSH32m, push32m ),

		// POP
		CCommand( copPOP8m,  pop8m  ),
		CCommand( copPOP16m, pop16m ),
		CCommand( copPOP32m, pop32m ),

		// SPOP
		CCommand( copSPOP,    spop    ),
		CCommand( copSPOP16v, spop16v ),
		CCommand( copSPOP16m, spop16m ),

		// LOCK
		CCommand( copLOCK,  lock ),
		CCommand( copLOCKm, lockm ),

		// CALL
		CCommand( copCALL32v, call32v ),
		CCommand( copCALL32m, call32m ),

		// RET
		CCommand( copRET8v,  ret8v ),
		CCommand( copRET8m,  ret8m ),
		CCommand( copRET16v, ret16v ),
		CCommand( copRET16m, ret16m ),
		CCommand( copRET32v, ret32v ),
		CCommand( copRET32m, ret32m ),

		CCommand( copRET, ret ),

		// -----------------------------------------------
		// Условные переходы
		// -----------------------------------------------

		// CMPE
		CCommand( copCMPE8mm,  cmpe8mm	),
		CCommand( copCMPE8mv,  cmpe8mv	),
		CCommand( copCMPE8vm,  cmpe8vm	),
		CCommand( copCMPE16mm, cmpe16mm	),
		CCommand( copCMPE16mv, cmpe16mv	),
		CCommand( copCMPE16vm, cmpe16vm	),
		CCommand( copCMPE32mm, cmpe32mm	),
		CCommand( copCMPE32mv, cmpe32mv	),
		CCommand( copCMPE32vm, cmpe32vm	),
		// CMPNE
		CCommand( copCMPNE8mm,  cmpne8mm	),
		CCommand( copCMPNE8mv,  cmpne8mv	),
		CCommand( copCMPNE8vm,  cmpne8vm	),
		CCommand( copCMPNE16mm, cmpne16mm	),
		CCommand( copCMPNE16mv, cmpne16mv	),
		CCommand( copCMPNE16vm, cmpne16vm	),
		CCommand( copCMPNE32mm, cmpne32mm	),
		CCommand( copCMPNE32mv, cmpne32mv	),
		CCommand( copCMPNE32vm, cmpne32vm	),
		// CMPB
		CCommand( copCMPB8mm,  cmpb8mm	),
		CCommand( copCMPB8mv,  cmpb8mv	),
		CCommand( copCMPB8vm,  cmpb8vm	),
		CCommand( copCMPB16mm, cmpb16mm	),
		CCommand( copCMPB16mv, cmpb16mv	),
		CCommand( copCMPB16vm, cmpb16vm	),
		CCommand( copCMPB32mm, cmpb32mm	),
		CCommand( copCMPB32mv, cmpb32mv	),
		CCommand( copCMPB32vm, cmpb32vm	),
		// CMPBE
		CCommand( copCMPBE8mm,  cmpbe8mm	),
		CCommand( copCMPBE8mv,  cmpbe8mv	),
		CCommand( copCMPBE8vm,  cmpbe8vm	),
		CCommand( copCMPBE16mm, cmpbe16mm	),
		CCommand( copCMPBE16mv, cmpbe16mv	),
		CCommand( copCMPBE16vm, cmpbe16vm	),
		CCommand( copCMPBE32mm, cmpbe32mm	),
		CCommand( copCMPBE32mv, cmpbe32mv	),
		CCommand( copCMPBE32vm, cmpbe32vm	),
		// CMPL
		CCommand( copCMPL8mm,  cmpl8mm	),
		CCommand( copCMPL8mv,  cmpl8mv	),
		CCommand( copCMPL8vm,  cmpl8vm	),
		CCommand( copCMPL16mm, cmpl16mm	),
		CCommand( copCMPL16mv, cmpl16mv	),
		CCommand( copCMPL16vm, cmpl16vm	),
		CCommand( copCMPL32mm, cmpl32mm	),
		CCommand( copCMPL32mv, cmpl32mv	),
		CCommand( copCMPL32vm, cmpl32vm	),
		// CMPLE
		CCommand( copCMPLE8mm,  cmple8mm	),
		CCommand( copCMPLE8mv,  cmple8mv	),
		CCommand( copCMPLE8vm,  cmple8vm	),
		CCommand( copCMPLE16mm, cmple16mm	),
		CCommand( copCMPLE16mv, cmple16mv	),
		CCommand( copCMPLE16vm, cmple16vm	),
		CCommand( copCMPLE32mm, cmple32mm	),
		CCommand( copCMPLE32mv, cmple32mv	),
		CCommand( copCMPLE32vm, cmple32vm	),
		// JCx
		CCommand( copJCT32v, jct32v ),
		CCommand( copJCT32m, jct32m ),
		CCommand( copJCF32v, jcf32v ),
		CCommand( copJCF32m, jcf32m ),

		//------------------------------------------------------------------------------
		// Внешние вызовы:
		//------------------------------------------------------------------------------

		// XLOCK
		CCommand( copXLOCK,  xlock  ),
		CCommand( copXLOCKm, xlockm ),
		// XCALL
		CCommand( copXCALL32v, xcall32v ),
		CCommand( copXCALL32m, xcall32m ),
		// XRET
		CCommand( copXRET,    xret    ),
		CCommand( copXRET8v,  xret8v  ),
		CCommand( copXRET8m,  xret8m  ),
		CCommand( copXRET16v, xret16v ),
		CCommand( copXRET16m, xret16m ),
		CCommand( copXRET32v, xret32v ),
		CCommand( copXRET32m, xret32m ),
		// LEA
		CCommand( copLEA32mfv, lea32mfv ),
		CCommand( copLEA32mfm, lea32mfm ),
		CCommand( copLEA32mv,  lea32mv  ),

		//------------------------------------------------------------------------------
		// Работа с указателями и смещениями
		//------------------------------------------------------------------------------

		// MOV-P
		CCommand( copMOV8mpo,   mov8mpo	  ),
		CCommand( copMOV16mpo,  mov16mpo  ),
		CCommand( copMOV32mpo,  mov32mpo  ),
		CCommand( copMOV8pom,   mov8pom	  ),
		CCommand( copMOV16pom,  mov16pom  ),
		CCommand( copMOV32pom,  mov32pom  ),							   
		CCommand( copMOV8pov,   mov8pov	  ),
		CCommand( copMOV16pov,  mov16pov  ),
		CCommand( copMOV32pov,  mov32pov  ),
		CCommand( copMOV8popo,  mov8popo  ),
		CCommand( copMOV16popo, mov16popo ),
		CCommand( copMOV32popo, mov32popo ),

		//------------------------------------------------------------------------------
		// Арифметические команды
		//------------------------------------------------------------------------------

		// ADD
		CCommand( copADD8mm,  add8mm  ),	
		CCommand( copADD8mv,  add8mv  ),	
		CCommand( copADD16mm, add16mm ),
		CCommand( copADD16mv, add16mv ),
		CCommand( copADD32mm, add32mm ),
		CCommand( copADD32mv, add32mv ),
		// SUB				  
		CCommand( copSUB8mm,  sub8mm  ),
		CCommand( copSUB8mv,  sub8mv  ),
		CCommand( copSUB16mm, sub16mm ),
		CCommand( copSUB16mv, sub16mv ),
		CCommand( copSUB32mm, sub32mm ),
		CCommand( copSUB32mv, sub32mv ),
		// MUL				  
		CCommand( copMUL8mm,  mul8mm  ),
		CCommand( copMUL8mv,  mul8mv  ),
		CCommand( copMUL16mm, mul16mm ),
		CCommand( copMUL16mv, mul16mv ),
		CCommand( copMUL32mm, mul32mm ),
		CCommand( copMUL32mv, mul32mv ),
		// IMUL				  
		CCommand( copIMUL8mm,  imul8mm  ),
		CCommand( copIMUL8mv,  imul8mv  ),
		CCommand( copIMUL16mm, imul16mm ),
		CCommand( copIMUL16mv, imul16mv ),
		CCommand( copIMUL32mm, imul32mm ),
		CCommand( copIMUL32mv, imul32mv ),
		// DIV				  
		CCommand( copDIV8mm,  div8mm  ),
		CCommand( copDIV8mv,  div8mv  ),
		CCommand( copDIV16mm, div16mm ),
		CCommand( copDIV16mv, div16mv ),
		CCommand( copDIV32mm, div32mm ),
		CCommand( copDIV32mv, div32mv ),
		// IDIV				  
		CCommand( copIDIV8mm,  idiv8mm  ),
		CCommand( copIDIV8mv,  idiv8mv  ),
		CCommand( copIDIV16mm, idiv16mm ),
		CCommand( copIDIV16mv, idiv16mv ),
		CCommand( copIDIV32mm, idiv32mm ),
		CCommand( copIDIV32mv, idiv32mv ),
		// NEG				  
		CCommand( copNEG8m,	 neg8m  ),
		CCommand( copNEG16m, neg16m ),
		CCommand( copNEG32m, neg32m ),

		//------------------------------------------------------------------------------
		// Логические команды
		//------------------------------------------------------------------------------

		// AND
		CCommand( copAND8mm,  and8mm  ),	
		CCommand( copAND8mv,  and8mv  ),
		CCommand( copAND16mm, and16mm ),
		CCommand( copAND16mv, and16mv ),
		CCommand( copAND32mm, and32mm ),
		CCommand( copAND32mv, and32mv ),
		// OR				 
		CCommand( copOR8mm,	 or8mm	),
		CCommand( copOR8mv,	 or8mv	),
		CCommand( copOR16mm, or16mm	),
		CCommand( copOR16mv, or16mv	),
		CCommand( copOR32mm, or32mm	),
		CCommand( copOR32mv, or32mv	),
		// XOR				 
		CCommand( copXOR8mm,  xor8mm  ),
		CCommand( copXOR8mv,  xor8mv  ),
		CCommand( copXOR16mm, xor16mm ),
		CCommand( copXOR16mv, xor16mv ),
		CCommand( copXOR32mm, xor32mm ),
		CCommand( copXOR32mv, xor32mv ),

		CCommand( copNOT8m,  not8m  ),
		CCommand( copNOT16m, not16m ),
		CCommand( copNOT32m, not32m ),

		// Признак конца списка команд
		CCommand( copLastCOP, 0 )
	};

	//------------------------------------------------------------------------------

}