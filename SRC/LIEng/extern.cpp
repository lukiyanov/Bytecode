#include "extern.h"
#include "ExternalCallManager.h"

namespace interpreter {

	////////////////////////////////////////////////////////////////////////////////
	// XLOCK
	// ������ LOCK, �� ������������� ���������� ��������� ������� � �����
	////////////////////////////////////////////////////////////////////////////////

	//------------------------------------------------------------------------------
 	void xlock  (CVMState& vmState)
	{
		vmState.m_vmProcessor.lock = 0;
		// ���������� ������ ����� �� ������ ����� ����� ��� ������������
		vmState.m_vmProcessor.dwStackSizeBeforeExternCall = vmState.m_callStack.GetSize();
	}

	//------------------------------------------------------------------------------
 	void xlockm (CVMState& vmState)
	{
		// ����� ��������� Read_32v, � �� Read_8m/Read_16m/Read_32m,
		// �.�. vmState.m_vmProcessor.lock ������ �� ���������� ����������
		// �����, � �������� �� ������ ����������������� ����� ������.
		vmState.m_vmProcessor.lock = vmState.Read_32v();

		vmState.m_vmProcessor.dwStackSizeBeforeExternCall = vmState.m_callStack.GetSize();
	}

	//------------------------------------------------------------------------------

	////////////////////////////////////////////////////////////////////////////////
	// XCALL
	// ���������� ����������� � ���� ���� ������ ���� ������ 4.
	// ������� �� ������������.
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
	// ������� �� callback'� �� ���������� ��� ������� �������.
	////////////////////////////////////////////////////////////////////////////////

	//------------------------------------------------------------------------------
	void xret    (CVMState& vmState)
	{
		// ���� � ����� �����, �� ���������, ��� ��� ������� �����.
		// ����� ���������, ��� ��� ���������� �����, �.�. � ����
		// ������ � ����� ������ �������������� ����� ��������.

		// ���������� ����� �������������� ��� ret
		if ( vmState.m_callStack.GetSize() )
		{
			const dword dwRetAddr = vmState.StackPopD();
			vmState.JumpTo( dwRetAddr );
		}

		// ������� ����� �������� � ���������� ������
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
		// ���������� ����� �������������� ��� ret
		if ( vmState.m_callStack.GetSize() )
		{
			// ���� �������� �����, �� �������� �������� ���������...
			if (vmState.m_vmProcessor.lock)
			{
				vmState.CheckMemoryAt( vmState.m_vmProcessor.lock, sizeof(TOpSize) );
				TOpSize* retvalue = ( TOpSize* ) vmState.GetVaFromOffset( vmState.m_vmProcessor.lock );

				*retvalue = value;
			}

			const dword dwRetAddr = vmState.StackPopD();
			vmState.JumpTo( dwRetAddr );
		}

		// ������� ����� �������� � ���������� ������
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
	// ��������� ����������� ������ ������� �������� � ������ �������.
	// ������������ ����� ��� ���������� ������ callback-�������.
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
	// lea var1, var2 ; � var2 - callbackprocID
	void lea32mfv (CVMState& vmState)
	{
		dword* var1 = vmState.Read_32m();
		const dword id = vmState.Read_32v();

		*var1 = vmState.m_pExternalCallManager->GetCallbackAddress( id );
	}

	//------------------------------------------------------------------------------
	// lea var1, var2 ; � var1 �������� ����� var2 (� �� ��������)
	// (������� ����� var2, ������� ��� ��� ������� ��� ������� ��������)
	// ������ �������� - ��������.
	void lea32mv (CVMState& vmState)
	{
		dword* var1 = vmState.Read_32m();
		const dword offset = vmState.Read_32v();

		*var1 = vmState.GetVaFromOffset( offset );
	}
	//------------------------------------------------------------------------------
}