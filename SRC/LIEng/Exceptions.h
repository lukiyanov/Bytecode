#pragma once
#include "typedefs.h"
#include <string>

namespace interpreter {

#ifdef ALLOW_CHECKS
	//------------------------------------------------------------------------------
	// ������������ � CInterpretException_ReferenceIsOutOfAllocatedMemory ���
	// �������� ���� ��������� � �������������� ������
	enum ETreatReason
	{				// ��������������, ��� ������:
		trRead,		// ������, ������� ����������� ���������
		trWrite,	// ������, ������� ����������� ��������
		trExecute	// ��� ��������
	};

	typedef unsigned __int64 RVA;

	////////////////////////////////////////////////////////////////////////////////
	//								����������
	////////////////////////////////////////////////////////////////////////////////

	//------------------------------------------------------------------------------
	// ���������� �������������� ��������
	struct CInterpretException
	{
		virtual std::string Message() = 0;
		virtual ~CInterpretException(){}
	};

	//------------------------------------------------------------------------------
	// �������� ����������� ��� ��������
	struct CInterpretException_UnknownCommand
		: public CInterpretException
	{
		RVA  m_rvaOffset;	// �������� ������� �� �������� ������ �������� P-����
		opcode m_opCode;	// ������������ �������

	public:
		CInterpretException_UnknownCommand
		(
			const RVA rvaOffset, const opcode opCode
		):
		m_rvaOffset(rvaOffset), m_opCode(opCode) {}

		std::string Message();
	};

	//------------------------------------------------------------------------------
	// ������� ��������� � ��������������� ������� ������ ����������� ������
	struct CInterpretException_ReferenceIsOutOfAllocatedMemory
		: public CInterpretException
	{
		RVA  m_rvaOffset;		// �������� ������� �� �������� ������ �������� P-����
		ETreatReason m_tReason;	// ����� �������������� ������������ ������ ������

	public:
		CInterpretException_ReferenceIsOutOfAllocatedMemory
		(
			const RVA rvaOffset,
			const ETreatReason tReason
		):
		m_rvaOffset(rvaOffset), m_tReason(tReason) {}

		std::string Message();
	};

	//------------------------------------------------------------------------------
	// ������� POP'���� �� ����� ������, ��� � ��� ����
	struct CInterpretException_StackIsEnded
		: public CInterpretException
	{
		dword m_wdCount;		// ���������� ����, ������� ������ POP'����
		dword m_dwStackSize;	// ���������� ����, ������� ���� � �����

	public:
		CInterpretException_StackIsEnded
		(
			const dword wdCount, const dword dwStackSize
		):
		m_wdCount(wdCount), m_dwStackSize(dwStackSize) {}

		std::string Message();
	};

	//------------------------------------------------------------------------------
	// ������� �������� ����� ��������������������� callback'�
	struct CInterpretException_UnknownCallbackID
		: public CInterpretException
	{
		dword m_dwMaxID;	// ����������� ���������� ID
		dword m_dwFoundID;	// ������������ ID

	public:
		CInterpretException_UnknownCallbackID
		(
			const dword dwMaxID, const dword dwFoundID
		):
		m_dwMaxID(dwMaxID), m_dwFoundID(dwFoundID) {}
		std::string Message();
	};

	//------------------------------------------------------------------------------
#endif
}