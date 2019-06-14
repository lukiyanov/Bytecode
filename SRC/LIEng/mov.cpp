#include "mov.h"

namespace interpreter {

	////////////////////////////////////////////////////////////////////////////////
	// MOV <��������>, <��������>
	// �����: �� �������� (� ���� �� �����������!)
	// ������������ �������: mov<������_���������><���_���������><���_���������>
	// <������_���������>: 
	//	8, 16, 32
	// ����� � ����� <���_*>:
	//	m - ������� � ������
	//	v - ���������������� ��������
	// ----------
	//	movT - ������, ������������ ����� ���������� (����������)
	// ----------
	// ������ ���������� MOV ������ ������������ �� ������� ������� ��������.
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