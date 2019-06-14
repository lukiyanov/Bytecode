#include "stack.h"

namespace interpreter {

	////////////////////////////////////////////////////////////////////////////////
	// PUSH
	////////////////////////////////////////////////////////////////////////////////

	//------------------------------------------------------------------------------
	void push8v (CVMState& vmState)
	{
		const byte value = vmState.Read_8v();

		vmState.StackPushB( value );
	}

	//------------------------------------------------------------------------------
	void push16v (CVMState& vmState)
	{
		const word value = vmState.Read_16v();

		vmState.StackPushW( value );
	}

	//------------------------------------------------------------------------------
	void push32v (CVMState& vmState)
	{
		const dword value = vmState.Read_32v();

		vmState.StackPushD( value );
	}

	//------------------------------------------------------------------------------
	void push8m (CVMState& vmState)
	{
		const byte value = *vmState.Read_8m();

		vmState.StackPushB( value );
	}

	//------------------------------------------------------------------------------
	void push16m (CVMState& vmState)
	{
		const word value = *vmState.Read_16m();

		vmState.StackPushW( value );
	}

	//------------------------------------------------------------------------------
	void push32m (CVMState& vmState)
	{
		const dword value = *vmState.Read_32m();

		vmState.StackPushD( value );
	}

	//------------------------------------------------------------------------------

	////////////////////////////////////////////////////////////////////////////////
	// POP
	////////////////////////////////////////////////////////////////////////////////

	//------------------------------------------------------------------------------
	void pop8m (CVMState& vmState)
	{
		byte* pMem = vmState.Read_8m();

		*pMem = vmState.StackPopB();
	}

	//------------------------------------------------------------------------------
	void pop16m (CVMState& vmState)
	{
		word* pMem = vmState.Read_16m();

		*pMem = vmState.StackPopW();
	}

	//------------------------------------------------------------------------------
	void pop32m (CVMState& vmState)
	{
		dword* pMem = vmState.Read_32m();

		*pMem = vmState.StackPopD();
	}

	//------------------------------------------------------------------------------

	////////////////////////////////////////////////////////////////////////////////
	// SPOP
	// (simple pop)
	// ¬ыталкивание значений из стека без записи их куда-либо
	////////////////////////////////////////////////////////////////////////////////

	//------------------------------------------------------------------------------
	void spop (CVMState& vmState)
	{
		vmState.SStackPop( 1 );
	}

	//------------------------------------------------------------------------------
	void spop16v (CVMState& vmState)
	{
		const word count = vmState.Read_16v();
		vmState.SStackPop( count );
	}

	//------------------------------------------------------------------------------
	void spop16m (CVMState& vmState)
	{
		const word count = *vmState.Read_16m();
		vmState.SStackPop( count );
	}

	////////////////////////////////////////////////////////////////////////////////
	// LOCK
	////////////////////////////////////////////////////////////////////////////////

	// LOCK устанавливает смещение, по которому записываетс€ результат от ret
	// с параметрами. /LOCK без параметров устанавливает этот адрес в 0 и в этом
	// случае ret с параметрами ничего никуда не записывает.
	// LOCK никак не вли€ет на ret без параметров.

	//------------------------------------------------------------------------------
 	void lock (CVMState& vmState)
	{
		vmState.m_vmProcessor.lock = 0;
	}

	//------------------------------------------------------------------------------
 	void lockm (CVMState& vmState)
	{
		// «десь применено Read_32v, а не Read_8m/Read_16m/Read_32m,
		// т.к. vmState.m_vmProcessor.lock хранит не абсолютный физический
		// адрес, а смещение от начала интерпретируемого блока пам€ти.
		vmState.m_vmProcessor.lock = vmState.Read_32v();
	}

	//------------------------------------------------------------------------------

	////////////////////////////////////////////////////////////////////////////////
	// CALL
	////////////////////////////////////////////////////////////////////////////////

	//------------------------------------------------------------------------------
	void call32v (CVMState& vmState)
	{
		const dword dwAddrOfFuncToCall = vmState.Read_32v();
		const dword dwAddrOfNextCmd = vmState.m_vmProcessor.eip;

		// —охран€ем в стек адрес возврата
		// (адрес следующей за нами команды)...
		vmState.StackPushD( dwAddrOfNextCmd );

		// ... и переходим к выполнению функции.
		vmState.JumpTo( dwAddrOfFuncToCall );
	}

	//------------------------------------------------------------------------------
	void call32m (CVMState& vmState)
	{
		const dword dwAddrOfFuncToCall = *vmState.Read_32m();
		const dword dwAddrOfNextCmd = vmState.m_vmProcessor.eip;

		// —охран€ем в стек адрес возврата
		// (адрес следующей за нами команды)...
		vmState.StackPushD( dwAddrOfNextCmd );

		// ... и переходим к выполнению функции.
		vmState.JumpTo( dwAddrOfFuncToCall );
	}

	//------------------------------------------------------------------------------

	////////////////////////////////////////////////////////////////////////////////
	// RET
	////////////////////////////////////////////////////////////////////////////////

	//------------------------------------------------------------------------------
	// >> return;
	//------------------------------------------------------------------------------
	// RET без параметров - просто вернутьс€ из функции к месту ее вызова
	void ret (CVMState& vmState)
	{
		const dword dwRetAddr = vmState.StackPopD();
		vmState.JumpTo( dwRetAddr );
	}

	//------------------------------------------------------------------------------
	// >> return value;
	//------------------------------------------------------------------------------
	// ≈сли lock == 0 то просто return;
	// »наче - записать в залоченную переменную (смещение которой находитс€ в lock)
	// значение, вз€тое из параметра (8/16/32, m/v),
	// после чего вернутьс€ из функции к месту ее вызова
	template <typename TOpSize>
	inline void retTv (CVMState& vmState, const TOpSize value)
	{
		// ≈сли известен адрес, по которому записать результат...
		if (vmState.m_vmProcessor.lock)
		{
			vmState.CheckMemoryAt( vmState.m_vmProcessor.lock, sizeof(TOpSize) );
			TOpSize* retvalue = ( TOpSize* ) vmState.GetVaFromOffset( vmState.m_vmProcessor.lock );

			*retvalue = value;
		}

		const dword dwRetAddr = vmState.StackPopD();
		vmState.JumpTo( dwRetAddr );
	}

	//------------------------------------------------------------------------------
	void ret8v (CVMState& vmState)
	{
		const byte value = vmState.Read_8v();
		retTv( vmState, value );
	}

	//------------------------------------------------------------------------------
	void ret8m (CVMState& vmState)
	{
		const byte value = *vmState.Read_8m();
		retTv( vmState, value );
	}

	//------------------------------------------------------------------------------
	void ret16v (CVMState& vmState)
	{
		const word value = vmState.Read_16v();
		retTv( vmState, value );
	}

	//------------------------------------------------------------------------------
	void ret16m (CVMState& vmState)
	{
		const word value = *vmState.Read_16m();
		retTv( vmState, value );
	}

	//------------------------------------------------------------------------------
	void ret32v (CVMState& vmState)
	{
		const dword value = vmState.Read_32v();
		retTv( vmState, value );
	}

	//------------------------------------------------------------------------------
	void ret32m (CVMState& vmState)
	{
		const dword value = *vmState.Read_32m();
		retTv( vmState, value );
	}

	//------------------------------------------------------------------------------

}