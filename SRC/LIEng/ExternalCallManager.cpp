#include "ExternalCallManager.h"
#include "Exceptions.h"
#include "Interpreter.h"
#include "windows.h"

namespace interpreter {
	//------------------------------------------------------------------------------
	CExternalCallManager::CExternalCallManager()
	{
	}

	//------------------------------------------------------------------------------
	CExternalCallManager::~CExternalCallManager(void)
	{
	}

	//------------------------------------------------------------------------------
	// �������� �� ������ ������� ��� ������� ������ Interpreter::Call � ���������������� ��.
	// ����� ����������� ����� ������ ������������

	byte* GenegateFunction(const dword dwFunctionOffset, CInterpreter* pThis, const dword dwDWFromStackToCopy)
	{
		// �������:
		static char codeOfCallbackProc[] =
			"\x54"					// push esp
			"\x68\x00\x00\x00\x00"	// push 00000000h
			"\x68\x00\x00\x00\x00"	// push 00000000h
			"\x68\x00\x00\x00\x00"	// push 00000000h
			"\xB8\x00\x00\x00\x00"	// mov eax, 00000000h
			"\xFF\xD0"				// call eax
			"\xC3";					// ret

		// ��������� �� �������, ������� �������� Callback'�
		dword (CExternalCallManager::*pPointer)(
			const interpreter::dword,
			CInterpreter*,
			const interpreter::dword,
			interpreter::dword)
		= &CExternalCallManager::Call;

		// ������� ������ ��� �������� ���������� ������ � ��������� ���� ������ ��������
		const dword dwShellCodeSize = sizeof(codeOfCallbackProc) - 1;
		byte* pFn = new byte[ dwShellCodeSize ];
		memcpy(pFn, codeOfCallbackProc, dwShellCodeSize);

		// ��������� �������� � ������
		CInterpreter* pthis = pThis;
		__asm
		{
			mov ebx, pFn

			mov eax, dwFunctionOffset
			mov dword ptr [ebx+2], eax	// �������� ������ ��������� functionOffset

			mov eax, pthis
			mov dword ptr [ebx+7], eax	// �������� ������ ������ this

			mov eax, dwDWFromStackToCopy
			mov dword ptr [ebx+12], eax	// �������� ������ ��������� dwDWFromStackToCopy

			mov eax, pPointer
			mov dword ptr [ebx+17], eax	// �������� ������ ������ Call
		}

		return pFn;
	}


	//------------------------------------------------------------------------------
	// �������� callback, ���������� P-��� �� �������� dwOffset � ������� ��� ID

	dword CExternalCallManager::RegisterCallback(const dword dwOffset, const dword dwCountOfDWordsToCopyFromStack)
	{
		// ������� �������
		byte* pFn = GenegateFunction(dwOffset, m_pInterpreter, dwCountOfDWordsToCopyFromStack);

		// �������� � ������ ������������������
		m_vctCallbackFunctions.push_back( pFn ); // (byte*)&CallbackTEST

		// ������� �� ID (������� � ����������)
		return m_vctCallbackFunctions.size() - 1;
	}

	//------------------------------------------------------------------------------
	dword CExternalCallManager::GetCallbackAddress(const dword dwID)
	{
	#ifdef ALLOW_CHECKS
		if (dwID >= m_vctCallbackFunctions.size())
		{
			throw CInterpretException_UnknownCallbackID( m_vctCallbackFunctions.size() - 1, dwID );
		}
	#endif

		const byte* p = m_vctCallbackFunctions[dwID];
		return (dword) p;
	}

	//------------------------------------------------------------------------------
	dword CExternalCallManager::Call(
		const dword		argumentCount,		// ���������� ������� ����, ������� ���������� � callback �� Win32.
		CInterpreter*	pOriginal,			// ��������� �� �������� CInterpreter, ������� ���������� ������ ExternalCallManager.
		const dword		functionOffset,		// �������� ������ ���������� ������� � ����-����.
		dword			esp					// ���������� ����������� �������� esp, ���������� ����� ���������� ���������� � ����.
	)
	{
		CInterpreter interp;
		interp.InitSession(
			pOriginal->m_vmCurrentState.m_vmMemory.m_pBlockBegin,
			pOriginal->m_vmCurrentState.m_vmMemory.m_dwSizeInBytes,
			functionOffset
		);

		// ���������� �����-��:
		dword* pStack = (dword*)interp.m_vmCurrentState.m_callStack.GetRoot();

		// ���������� ����� ��������
		esp += sizeof(dword) * (argumentCount); // sizeof(dword) * (argumentCount) = ������ ���������� � �����

		// ��������� ��������� � ���� VM. ����� �������� �� ���������� (��.�������� xret)
		for (dword i = 0; i < argumentCount; i++)
		{
			dword* pdw = (dword*)( esp - i * sizeof(dword) );
			interp.m_vmCurrentState.m_callStack.PushDWord( *pdw );
		}

		// �������� ���������� �� callback

#ifdef ALLOW_CHECKS
		try
		{
#endif
			interp.Run();
#ifdef ALLOW_CHECKS
		}
		catch (interpreter::CInterpretException& e)
		{
			MessageBoxA(0, e.Message().c_str(), "exception", MB_ICONWARNING);
		}
		catch (std::string str)
		{
			MessageBoxA(0, str.c_str(), "exception", MB_ICONWARNING);
		}
#endif
		// ���� �������� ����������...

		// ���������� �� ��� ������� ������� xret
		return interp.m_vmCurrentState.m_vmProcessor.lock;
	}

	//------------------------------------------------------------------------------

	//------------------------------------------------------------------------------
	// ������ ���������� ������ ��������� ������� � ��������� ������ � ����������
	// ����� ������ � �������. ���������� ������� ������������ ������ � �������.
	dword CExternalCallManager::RegisterExternalFunction(const char* pszModuleName, const char* pszFunctionName)
	{
		HMODULE hModule = LoadLibraryA(pszModuleName);
	#ifdef ALLOW_CHECKS
		if (!hModule)
		{
			std::string tmp = "Cannot load module: module=\"";
			tmp += pszModuleName;
			tmp += '\"';
			throw tmp;
		}
	#endif

		FARPROC pFn = GetProcAddress(hModule, pszFunctionName);
	#ifdef ALLOW_CHECKS
		if (!pFn)
		{
			std::string tmp = "Cannot find function: module=\"";
			tmp += pszModuleName;
			tmp += "\", name=\"";
			tmp += pszFunctionName;
			tmp += "\"";
			throw tmp;
		}
	#endif

		m_vctExternFunctions.push_back( (void*)pFn );
		return m_vctExternFunctions.size() - 1;
	}

	//------------------------------------------------------------------------------
	// TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO
	void CExternalCallManager::CallExternalFunction(const dword dwID)
	{
		CVMState& vmState = m_pInterpreter->m_vmCurrentState;

		const dword dwStackSizeBeforeExternCall = vmState.m_vmProcessor.dwStackSizeBeforeExternCall;	// � ������.
		const dword dwFullStackSize             = vmState.m_callStack.GetSize();						// ���������� ����, ������� ������������ ������.
		const dword dwCountOfBytes              = dwFullStackSize - dwStackSizeBeforeExternCall;		// ������ � ������ ����� ��� ������� � ���� ���������� ������� ������� ���.
		const dword dwCountOfDWORDs             = dwCountOfBytes / 4;									// ������ ���� �� � ������� ������.

		void* p = m_vctExternFunctions[dwID];	// �������, ������� ����� ��������.
		dword dwResult;							// �� ��� ��� ����� � �������� eax, ����������� �����.
		

		/*
		dword i;
		dword d;
		for (i = 0; i < dwCountOfDWORDs; i++)
		{
			d = *(dwInitialStackTop + i);
			__asm push d
		}*/

		// ����������, �����:
		const dword* pFirstArgAddrInStack = (dword*)(vmState.m_callStack.GetRoot() + dwStackSizeBeforeExternCall);
		__asm
		{
			// �������� ��������� �� ����� ����������� ������ � ����������
			xor eax, eax						// i = 0
			loop_beg:
				cmp eax, dwCountOfDWORDs
				je end_loop

				mov  ebx, eax					// \ 
				imul ebx, 4						// - ebx = pFirstArgAddrInStack + i * sizeof(dword*)
				add  ebx, pFirstArgAddrInStack	// /
				mov ebx, [ebx]					// ����� ���������, �� ��� ��� �������.
				push ebx						// push( *ebx )

				inc eax							// i++
				jmp loop_beg
			end_loop:

			// ���� ��� ����������� callback'a ���������� ����� � ���� �����, �� �������� ��� ������
			// ��� ����� ��������� ר���� DEP, ��������������� ���������� ������.
			call p
			mov dwResult, eax

			// ������� ���� �� ����
		}

		// ��������������� ������ �����, ������� ��� ������� ����� ���� �������� xlock
		vmState.m_callStack.SetSize( dwStackSizeBeforeExternCall );

		// ���� ������������ �������� �������� ���������
		// ������� � ���������, ������� ��� �������� �� ����������� ��������.
		if (vmState.m_vmProcessor.lock)
		{
			vmState.CheckMemoryAt( vmState.m_vmProcessor.lock, 4 );
			dword *pDW = (dword*)vmState.GetVaFromOffset( vmState.m_vmProcessor.lock );
			*pDW = dwResult;
		}
	}

	//------------------------------------------------------------------------------
	std::vector<void*> CExternalCallManager::m_vctExternFunctions;
	std::vector<byte*> CExternalCallManager::m_vctCallbackFunctions;
}