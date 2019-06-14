#include "jmp.h"

namespace interpreter {

	////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////
	// MOV <��������>, <��������>
	// �����: �� ��������
	// ������������ �������: mov<������_���������><���_���������><���_���������>
	// <������_���������>: 
	//	16x8, 32x8, 32x16
	// ����� � ����� <���_*>:
	//	m - ������� � ������
	////////////////////////////////////////////////////////////////////////////////

	// MOVSX
	//------------------------------------------------------------------------------
	void movsx16x8mm  (CVMState& vmState)
	{
		word* pSrc = vmState.Read_16m();
		byte dst = *vmState.Read_8m();
		__asm {
			mov edx, pSrc
			movsx ax, dst
			mov word ptr [edx], ax
		}
	}

	//------------------------------------------------------------------------------
	void movsx32x8mm  (CVMState& vmState)
	{
		dword* pSrc = vmState.Read_32m();
		byte dst = *vmState.Read_8m();
		__asm {
			mov edx, pSrc
			movsx eax, dst
			mov dword ptr [edx], eax
		}
	}

	//------------------------------------------------------------------------------
	void movsx32x16mm (CVMState& vmState)
	{
		dword* pSrc = vmState.Read_32m();
		word dst = *vmState.Read_16m();
		__asm {
			mov edx, pSrc
			movsx eax, dst
			mov dword ptr [edx], eax
		}
	}


	// MOVSZ
	//------------------------------------------------------------------------------
	void movzx16x8mm  (CVMState& vmState)
	{
		word* pSrc = vmState.Read_16m();
		byte dst = *vmState.Read_8m();
		__asm {
			mov edx, pSrc
			movzx ax, dst
			mov word ptr [edx], ax
		}
	}

	//------------------------------------------------------------------------------
	void movzx32x8mm  (CVMState& vmState)
	{
		dword* pSrc = vmState.Read_32m();
		byte dst = *vmState.Read_8m();
		__asm {
			mov edx, pSrc
			movzx eax, dst
			mov dword ptr [edx], eax
		}
	}

	//------------------------------------------------------------------------------
	void movzx32x16mm (CVMState& vmState)
	{
		dword* pSrc = vmState.Read_32m();
		word dst = *vmState.Read_16m();
		__asm {
			mov edx, pSrc
			movzx eax, dst
			mov dword ptr [edx], eax
		}
	}

	//------------------------------------------------------------------------------
}