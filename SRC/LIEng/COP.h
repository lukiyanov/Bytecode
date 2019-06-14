#pragma once

namespace interpreter {

	enum ECOP
	{
		//------------------------------------------------------------------------------
		// Generic:
		//------------------------------------------------------------------------------
		// NOP - Сразу переводит eip на следующую инструкцию
		copNOP = 0,
		// HALT - Корректно завершает выполнение программы
		copHALT = 1,
		// BRK - Брейкпоинт, переводит eip на следующую команду и передает управление
		// на внешний обработчик прерывания
		copBRK,

		//------------------------------------------------------------------------------
		// MOV:
		//------------------------------------------------------------------------------
		copMOV8mm,
		copMOV8mv,
		copMOV16mm,
		copMOV16mv,
		copMOV32mm,
		copMOV32mv,

		//------------------------------------------------------------------------------
		// JMP:
		//------------------------------------------------------------------------------
		copJMP32m,
		copJMP32v,

		//------------------------------------------------------------------------------
		// MOVXX:
		//------------------------------------------------------------------------------
		// MOVSX
		copMOVSX16x8mm,
		copMOVSX32x8mm,
		copMOVSX32x16mm,
		// MOVZX
		copMOVZX16x8mm,
		copMOVZX32x8mm,
		copMOVZX32x16mm,

		//------------------------------------------------------------------------------
		// Работа со стеком:
		//------------------------------------------------------------------------------
		// PUSH
		copPUSH8v,
		copPUSH16v,
		copPUSH32v,
		copPUSH8m,
		copPUSH16m,
		copPUSH32m,
		// POP
		copPOP8m,
		copPOP16m,
		copPOP32m,
		// SPOP
		copSPOP,
		copSPOP16v,
		copSPOP16m,
		// LOCK
		copLOCK,
		copLOCKm,
		// CALL
		copCALL32v,
		copCALL32m,
		// RET
		copRET,
		copRET8v,
		copRET8m,
		copRET16v,
		copRET16m,
		copRET32v,
		copRET32m,

		//------------------------------------------------------------------------------
		// Условные переходы (Jxx):
		//------------------------------------------------------------------------------
		// CMPE
		copCMPE8mm,
		copCMPE8mv,
		copCMPE8vm,
		copCMPE16mm,
		copCMPE16mv,
		copCMPE16vm,
		copCMPE32mm,
		copCMPE32mv,
		copCMPE32vm,
		// CMPNE
		copCMPNE8mm,
		copCMPNE8mv,
		copCMPNE8vm,
		copCMPNE16mm,
		copCMPNE16mv,
		copCMPNE16vm,
		copCMPNE32mm,
		copCMPNE32mv,
		copCMPNE32vm,
		// CMPB
		copCMPB8mm,
		copCMPB8mv,
		copCMPB8vm,
		copCMPB16mm,
		copCMPB16mv,
		copCMPB16vm,
		copCMPB32mm,
		copCMPB32mv,
		copCMPB32vm,
		// CMPBE
		copCMPBE8mm,
		copCMPBE8mv,
		copCMPBE8vm,
		copCMPBE16mm,
		copCMPBE16mv,
		copCMPBE16vm,
		copCMPBE32mm,
		copCMPBE32mv,
		copCMPBE32vm,
		// CMPL
		copCMPL8mm,
		copCMPL8mv,
		copCMPL8vm,
		copCMPL16mm,
		copCMPL16mv,
		copCMPL16vm,
		copCMPL32mm,
		copCMPL32mv,
		copCMPL32vm,
		// CMPLE
		copCMPLE8mm,
		copCMPLE8mv,
		copCMPLE8vm,
		copCMPLE16mm,
		copCMPLE16mv,
		copCMPLE16vm,
		copCMPLE32mm,
		copCMPLE32mv,
		copCMPLE32vm,
		// JCx
		copJCT32v,
		copJCT32m,
		copJCF32v,
		copJCF32m,

		//------------------------------------------------------------------------------
		// Внешние вызовы:
		//------------------------------------------------------------------------------
		// XLOCK
		copXLOCK,
		copXLOCKm,
		// XCALL
		copXCALL32v,
		copXCALL32m,
		// XRET
		copXRET,
		copXRET8v,
		copXRET8m,
		copXRET16v,
		copXRET16m,
		copXRET32v,
		copXRET32m,
		// LEA
		copLEA32mfv,
		copLEA32mfm,
		copLEA32mv,

		//------------------------------------------------------------------------------
		// Работа с указателями и смещениями
		//------------------------------------------------------------------------------
		// MOV-P:
		// MOV-P ( m = *(p+o) )
		copMOV8mpo,
		copMOV16mpo,
		copMOV32mpo,
		// MOV-P ( *(p+o) = m )
		copMOV8pom,
		copMOV16pom,
		copMOV32pom,
		// MOV-P ( *(p+o) = v )
		copMOV8pov,
		copMOV16pov,
		copMOV32pov,
		// MOV-P ( *(p1+o1) = *(p2+o2) )
		copMOV8popo,
		copMOV16popo,
		copMOV32popo,

		//------------------------------------------------------------------------------
		// Арифметические команды
		//------------------------------------------------------------------------------
		// ADD
		copADD8mm,
		copADD8mv,
		copADD16mm,
		copADD16mv,
		copADD32mm,
		copADD32mv,
		// SUB
		copSUB8mm,
		copSUB8mv,
		copSUB16mm,
		copSUB16mv,
		copSUB32mm,
		copSUB32mv,
		// MUL
		copMUL8mm,
		copMUL8mv,
		copMUL16mm,
		copMUL16mv,
		copMUL32mm,
		copMUL32mv,
		// IMUL
		copIMUL8mm,
		copIMUL8mv,
		copIMUL16mm,
		copIMUL16mv,
		copIMUL32mm,
		copIMUL32mv,
		// DIV
		copDIV8mm,
		copDIV8mv,
		copDIV16mm,
		copDIV16mv,
		copDIV32mm,
		copDIV32mv,
		// IDIV
		copIDIV8mm,
		copIDIV8mv,
		copIDIV16mm,
		copIDIV16mv,
		copIDIV32mm,
		copIDIV32mv,
		// NEG
		copNEG8m,
		copNEG16m,
		copNEG32m,
	
		//------------------------------------------------------------------------------
		// Логические команды
		//------------------------------------------------------------------------------

		// AND
		copAND8mm,
		copAND8mv,
		copAND16mm,
		copAND16mv,
		copAND32mm,
		copAND32mv,
		// OR
		copOR8mm,
		copOR8mv,
		copOR16mm,
		copOR16mv,
		copOR32mm,
		copOR32mv,
		// XOR
		copXOR8mm,
		copXOR8mv,
		copXOR16mm,
		copXOR16mv,
		copXOR32mm,
		copXOR32mv,

		copNOT8m,
		copNOT16m,
		copNOT32m,

		//------------------------------------------------------------------------------

		// Не команда, применяется для вычисления к-ва COP'ов
		copLastCOP
	};

}
