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

	// На случай, если команды идут в иной последовательности (перемешаны)
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
	// Задача - добиться от функции максимального быстродействия

	const dword dwEIP            = m_vmCurrentState.m_vmProcessor.eip;
	const dword dwSizeOfMemBlock = m_vmCurrentState.m_vmMemory.m_dwSizeInBytes;

#ifdef ALLOW_CHECKS
	// Проверка на выход за пределы выделенной памяти.
	// (dwEIP + sizeof(opcode) - 1) - смещение последнего байта опкода
	// текущей команды.
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
	// Проверка на неизвестную команду
	if ( opCode >= copLastCOP )
	{
		throw CInterpretException_UnknownCommand( dwEIP, opCode );
	}
#endif

	// Увеличим eip, чтобы он указывал на первый байт параметра команды
	// или следующую команду, если параметров у команды нет.
	m_vmCurrentState.m_vmProcessor.eip += sizeof(opcode);

	// Вызов функции - обработчика команды opCode
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