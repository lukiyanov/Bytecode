#pragma once
#include "typedefs.h"
#include "Command.h"
#include "VMState.h"
#include "ExternalCallManager.h"

#include <vector>

namespace interpreter {
////////////////////////////////////////////////////////////////////////////////

	class CInterpreter
	{
		friend class CExternalCallManager;
	public:
		CInterpreter(void);
		~CInterpreter(void);

		void InitSession(byte* pCode, const dword dwSizeOfCode, const dword dwEntryPointOffset);
		void ExecuteCommand();
		void Run();


		bool IsVMWorking() const
			{ return m_vmCurrentState.m_bStillWorking; }

	private:
		CVMState m_vmCurrentState;				// ������� ��������� ����������� ������
		std::vector<CCommand> m_vctCommandList;	// ����������� ��������� ������

	// --------------------------
	// ���������� ������� ������� - ������������� � ������� CVMState
	public:
		// ������� ������������ ������� ������� � ���������� �� ID.
		// ������ ���� �������� �� Interpreter.Run().
		// � ���������� ������ ����� ���������� ���������� 0x0101 � 0x0102.
		inline dword RegisterExternalFunction(const char* pszModuleName, const char* pszFunctionName)
		{
			return m_extManager.RegisterExternalFunction( pszModuleName, pszFunctionName );
		}

		// �������� callback-�������, ������� �������� P-�������
		// �� �������� dwOffset.
		// ���������� ID ������������������ �������.
		// ������ ���� �������� �� Interpreter.Run().
		inline dword RegisterCallback(const dword dwOffset, const dword dwCountOfDWordsToCopyFromStack)
		{
			return m_extManager.RegisterCallback( dwOffset, dwCountOfDWordsToCopyFromStack );
		}

	private:
		CExternalCallManager m_extManager;
	};

////////////////////////////////////////////////////////////////////////////////
}