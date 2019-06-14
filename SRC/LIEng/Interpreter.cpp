#include "Interpreter.h"
#include "Command.h"
#include "Exceptions.h"
#include "COP.h"
namespace interpreter
{

//------------------------------------------------------------------------------
CInterpreter::CInterpreter(void):
	m_vctCommandList( copLastCOP )
{
	m_extManager.Init( this );

	m_vctCommandList.reserve( (int)copLastCOP - 1 );

	// �� ������, ���� ������� ���� � ���� ������������������ (����������)
	for (int i = 0; aRegisteredCommands[i].pfn; i++)
	{
		m_vctCommandList[ aRegisteredCommands[i].COP ] = aRegisteredCommands[i];
	}
}

//------------------------------------------------------------------------------
CInterpreter::~CInterpreter(void)
{
}

//------------------------------------------------------------------------------
void CInterpreter::InitSession(byte* pCode, const dword dwSizeOfCode, const dword dwEntryPointOffset)
{
	m_vmCurrentState.Init(pCode, dwSizeOfCode, dwEntryPointOffset, &m_extManager);
}

//------------------------------------------------------------------------------
void CInterpreter::ExecuteCommand()
{
	// ������ - �������� �� ������� ������������� ��������������

	const dword dwEIP            = m_vmCurrentState.m_vmProcessor.eip;
	const dword dwSizeOfMemBlock = m_vmCurrentState.m_vmMemory.m_dwSizeInBytes;

#ifdef ALLOW_CHECKS
	// �������� �� ����� �� ������� ���������� ������.
	// (dwEIP + sizeof(opcode) - 1) - �������� ���������� ����� ������
	// ������� �������.
	if ( (dwEIP + sizeof(opcode) - 1) >= dwSizeOfMemBlock	)
	{
		throw CInterpretException_ReferenceIsOutOfAllocatedMemory( dwEIP, trExecute	);
	}
#endif

	const dword dwBaseOfMemBlock = (dword) m_vmCurrentState.m_vmMemory.m_pBlockBegin;
	const dword dwRealCOPAddress = dwBaseOfMemBlock + dwEIP;

	opcode opCode = *( (word*) dwRealCOPAddress );
	ECOP COP = (ECOP) opCode;

#ifdef ALLOW_CHECKS
	// �������� �� ����������� �������
	if ( opCode >= copLastCOP )
	{
		throw CInterpretException_UnknownCommand( dwEIP, opCode );
	}
#endif

	// �������� eip, ����� �� �������� �� ������ ���� ��������� �������
	// ��� ��������� �������, ���� ���������� � ������� ���.
	m_vmCurrentState.m_vmProcessor.eip += sizeof(opcode);

	// ����� ������� - ����������� ������� opCode
	m_vctCommandList[opCode].pfn( m_vmCurrentState );
}

//------------------------------------------------------------------------------
void CInterpreter::Run()
{
	while ( IsVMWorking() )
	{
		ExecuteCommand();
	}
}

//------------------------------------------------------------------------------

}