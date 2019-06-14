#include "jmp.h"

namespace interpreter {

	////////////////////////////////////////////////////////////////////////////////
	// JMP <��������>
	// ������������ �������: jmp<������_���������><���_���������>
	// <������_���������>: 
	//	32
	// <���_���������>:
	//	m - ������� � ������
	//	v - ���������������� ��������
	////////////////////////////////////////////////////////////////////////////////

	//------------------------------------------------------------------------------
	void jmp32m (CVMState& vmState)
	{
		dword* memSrc = vmState.Read_32m();
		vmState.JumpTo( *memSrc );
	}

	//------------------------------------------------------------------------------
	void jmp32v (CVMState& vmState)
	{
		const dword valSrc = vmState.Read_32v();
		vmState.JumpTo( valSrc );
	}

	//------------------------------------------------------------------------------
}