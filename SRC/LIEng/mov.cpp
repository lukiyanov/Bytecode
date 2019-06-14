#include "mov.h"

namespace interpreter {

	////////////////////////////////////////////////////////////////////////////////
	// MOV <приемник>, <источник>
	// Флаги: не изменяет (у меня не поизменяешь!)
	// Формирование команды: mov<размер_операндов><тип_приемника><тип_источника>
	// <размер_операндов>: 
	//	8, 16, 32
	// Здесь и далее <тип_*>:
	//	m - операнд в памяти
	//	v - непосредственное значение
	// ----------
	//	movT - шаблон, используется всеми остальными (адаптерами)
	// ----------
	// Размер инструкции MOV всегда определяется по размеру первого операнда.
	////////////////////////////////////////////////////////////////////////////////

	//------------------------------------------------------------------------------
	template <typename T>
	inline void movT ( T* opDest, const T opSrc )
	{
		*opDest = opSrc;
	}

	//------------------------------------------------------------------------------
	void mov8mm (CVMState& vmState)
	{
		byte* memDst = vmState.Read_8m();
		byte* memSrc = vmState.Read_8m();
		movT( memDst, *memSrc );
	}

	//------------------------------------------------------------------------------
	void mov8mv (CVMState& vmState)
	{
		byte* memDst = vmState.Read_8m();
		const byte valSrc = vmState.Read_8v();
		movT( memDst, valSrc );
	}

	//------------------------------------------------------------------------------
	void mov16mm (CVMState& vmState)
	{
		word* memDst = vmState.Read_16m();
		word* memSrc = vmState.Read_16m();
		movT( memDst, *memSrc );
	}

	//------------------------------------------------------------------------------
	void mov16mv (CVMState& vmState)
	{
		word* memDst = vmState.Read_16m();
		const word valSrc = vmState.Read_16v();
		movT( memDst, valSrc );
	}

	//------------------------------------------------------------------------------
	void mov32mm (CVMState& vmState)
	{
		dword* memDst = vmState.Read_32m();
		dword* memSrc = vmState.Read_32m();
		movT( memDst, *memSrc );
	}

	//------------------------------------------------------------------------------
	void mov32mv (CVMState& vmState)
	{
		dword* memDst = vmState.Read_32m();
		const dword valSrc = vmState.Read_32v();
		movT( memDst, valSrc );
	}

	//------------------------------------------------------------------------------

}