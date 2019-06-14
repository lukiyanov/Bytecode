#pragma once

#include <vector>
#include "typedefs.h"

namespace interpreter {

	class CInterpreter;

	// �������� ������� �������. �������� �� ����� � Windows.
	class CExternalCallManager
	{
	public:
		CExternalCallManager();
		~CExternalCallManager(void);
		inline void Init(CInterpreter* pInterpreter)
		{
			m_pInterpreter = pInterpreter;
		}

	public:
		//------------------------------------------------------------------------------
		// ���������� ������� �������

		// �������� callback-�������, ������� �������� P-�������
		// �� �������� dwOffset.
		// ���������� ID ������������������ �������.
		// ������ ���� �������� �� Interpretep.Run().
		dword RegisterCallback(const dword dwOffset, const dword dwCountOfDWordsToCopyFromStack);

		// ������� ������������ ������� ������� � ���������� �� ID.
		// � ���������� ������ ����� ���������� ���������� 0x0101 � 0x0102.
		// ������ ���� �������� �� Interpreter.Run().
		dword RegisterExternalFunction(const char* pszModuleName, const char* pszFunctionName);

		// ��������� ����������� ������ callback'a �� ��� ID.
		// ������������ � ������� lea.
		dword GetCallbackAddress(const dword dwID);

		// �������� ������������������ ������� ������� �� �� ID.
		// (� �.�. ��������� ��������� ����)
		// ������������ � ������� xcall.
		void CallExternalFunction(const dword dwID);

	public:
		// ����������� ����� P-������� �� ��������� dwOffset.
		// ���������� �� �������-callback'��.
		dword Call(
			const dword dwCountOfDWordsToCopyFromStack,
			CInterpreter* pThis,
			const dword dwOffset,
			dword esp
		);

	private:
		// ��� �������������� ����������� � callback'�� �������� ������
		// (����� callback �� ������ ������������ �������� ���������),
		// ��� ������� ������������.
		static std::vector<void*> m_vctExternFunctions;
		static std::vector<byte*> m_vctCallbackFunctions;
		CInterpreter* m_pInterpreter;
	};

}
