#include "extern.h"
#include "ExternalCallManager.h"

namespace interpreter {

	////////////////////////////////////////////////////////////////////////////////
	// XLOCK
	// Аналог LOCK, но дополнительно запоминает начальную позицию в стеке
	////////////////////////////////////////////////////////////////////////////////

	//------------------------------------------------------------------------------
 	void xlock  (CVMState& vmState)
	{
		vmState.m_vmProcessor.lock = 0;
		// Записываем размер стека до вызова чтобы потом его восстановить
		vmState.m_vmProcessor.dwStackSizeBeforeExternCall = vmState.m_callStack.GetSize();
	}

	//------------------------------------------------------------------------------
 	void xlockm (CVMState& vmState)
	{
		// Здесь применено Read_32v, а не Read_8m/Read_16m/Read_32m,
		// т.к. vmState.m_vmProcessor.lock хранит не абсолютный физический
		// адрес, а смещение от начала интерпретируемого блока памяти.
		vmState.m_vmProcessor.lock = vmState.Read_32v();

		vmState.m_vmProcessor.dwStackSizeBeforeExternCall = vmState.m_callStack.GetSize();
	}

	//------------------------------------------------------------------------------

	////////////////////////////////////////////////////////////////////////////////
	// XCALL
	// Количество добавленных в стек байт должно быть кратно 4.
	// Остаток не записывается.
	////////////////////////////////////////////////////////////////////////////////

	//------------------------------------------------------------------------------
	void xcall32v (CVMState& vmState)
	{
		const dword funcID = vmState.Read_32v();

		vmState.m_pExternalCallManager->CallExternalFunction( funcID );
	}

	//------------------------------------------------------------------------------
	void xcall32m (CVMState& vmState)
	{
		const dword funcID = *vmState.Read_32m();

		vmState.m_pExternalCallManager->CallExternalFunction( funcID );
	}

	//------------------------------------------------------------------------------

	////////////////////////////////////////////////////////////////////////////////
	// XRET
	// Возврат из callback'а во внутреннюю или внешнюю функцию.
	////////////////////////////////////////////////////////////////////////////////

	//------------------------------------------------------------------------------
	void xret    (CVMState& vmState)
	{
		// Если в стеке пусто, то считается, что это внешний вызов.
		// Иначе считается, что это внутренний вызов, т.к. в этом
		// случае в стеке должен присутствовать адрес возврата.

		// Внутренний вызов обрабатывается как ret
		if ( vmState.m_callStack.GetSize() )
		{
			const dword dwRetAddr = vmState.StackPopD();
			vmState.JumpTo( dwRetAddr );
		}

		// Внешний вызов приводит к завершению сессии
		else
		{
			vmState.m_vmProcessor.lock = 0;
			vmState.m_bStillWorking = 0;	// halt
		}
	}

	//------------------------------------------------------------------------------
	template <typename TOpSize>
		inline void xretT (CVMState& vmState, const TOpSize value)
	{
		// Внутренний вызов обрабатывается как ret
		if ( vmState.m_callStack.GetSize() )
		{
			// Если известен адрес, по которому записать результат...
			if (vmState.m_vmProcessor.lock)
			{
				vmState.CheckMemoryAt( vmState.m_vmProcessor.lock, sizeof(TOpSize) );
				TOpSize* retvalue = ( TOpSize* ) vmState.GetVaFromOffset( vmState.m_vmProcessor.lock );

				*retvalue = value;
			}

			const dword dwRetAddr = vmState.StackPopD();
			vmState.JumpTo( dwRetAddr );
		}

		// Внешний вызов приводит к завершению сессии
		else
		{
			vmState.m_vmProcessor.lock = value;
			vmState.m_bStillWorking = 0;	// halt
		}
	}

	//------------------------------------------------------------------------------
	void xret8v  (CVMState& vmState)
	{
		const byte value = vmState.Read_8v();
		xretT(vmState, value);
	}

	//------------------------------------------------------------------------------
	void xret8m  (CVMState& vmState)
	{
		const byte value = *vmState.Read_8m();
		xretT(vmState, value);
	}

	//------------------------------------------------------------------------------
	void xret16v (CVMState& vmState)
	{
		const word value = vmState.Read_16v();
		xretT(vmState, value);
	}

	//------------------------------------------------------------------------------
	void xret16m (CVMState& vmState)
	{
		const word value = *vmState.Read_16m();
		xretT(vmState, value);
	}

	//------------------------------------------------------------------------------
	void xret32v (CVMState& vmState)
	{
		const dword value = vmState.Read_32v();
		xretT(vmState, value);
	}

	//------------------------------------------------------------------------------
	void xret32m (CVMState& vmState)
	{
		const dword value = *vmState.Read_32m();
		xretT(vmState, value);
	}

	//------------------------------------------------------------------------------

	////////////////////////////////////////////////////////////////////////////////
	// LEA
	////////////////////////////////////////////////////////////////////////////////
	// Пересылка физического адреса второго операнда в первый операнд.
	// Используется также для вычисления адреса callback-функции.
	////////////////////////////////////////////////////////////////////////////////

	//------------------------------------------------------------------------------
	// lea var1, callbackprocID
	void lea32mfm (CVMState& vmState)
	{
		dword* var1 = vmState.Read_32m();
		const dword id = *vmState.Read_32m();

		*var1 = vmState.m_pExternalCallManager->GetCallbackAddress( id );
	}

	//------------------------------------------------------------------------------
	// lea var1, var2 ; в var2 - callbackprocID
	void lea32mfv (CVMState& vmState)
	{
		dword* var1 = vmState.Read_32m();
		const dword id = vmState.Read_32v();

		*var1 = vmState.m_pExternalCallManager->GetCallbackAddress( id );
	}

	//------------------------------------------------------------------------------
	// lea var1, var2 ; в var1 реальный адрес var2 (а не смещение)
	// (первого байта var2, поэтому тут без разницы тип второго операнда)
	// Второй параметр - смещение.
	void lea32mv (CVMState& vmState)
	{
		dword* var1 = vmState.Read_32m();
		const dword offset = vmState.Read_32v();

		*var1 = vmState.GetVaFromOffset( offset );
	}
	//------------------------------------------------------------------------------
}