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
	// Выделить из памяти область для функции вызова Interpreter::Call и инициализировать ее.
	// Адрес выделенного блока памяти возвращается

	byte* GenegateFunction(const dword dwFunctionOffset, CInterpreter* pThis, const dword dwDWFromStackToCopy)
	{
		// Шеллкод:
		static char codeOfCallbackProc[] =
			"\x54"					// push esp
			"\x68\x00\x00\x00\x00"	// push 00000000h
			"\x68\x00\x00\x00\x00"	// push 00000000h
			"\x68\x00\x00\x00\x00"	// push 00000000h
			"\xB8\x00\x00\x00\x00"	// mov eax, 00000000h
			"\xFF\xD0"				// call eax
			"\xC3";					// ret

		// Указатель на функцию, которую вызывают Callback'и
		dword (CExternalCallManager::*pPointer)(
			const interpreter::dword,
			CInterpreter*,
			const interpreter::dword,
			interpreter::dword)
		= &CExternalCallManager::Call;

		// Выделим нужное для шеллкода количество памяти и скопируем туда шаблон шеллкода
		const dword dwShellCodeSize = sizeof(codeOfCallbackProc) - 1;
		byte* pFn = new byte[ dwShellCodeSize ];
		memcpy(pFn, codeOfCallbackProc, dwShellCodeSize);

		// Установим значения в шаблон
		CInterpreter* pthis = pThis;
		__asm
		{
			mov ebx, pFn

			mov eax, dwFunctionOffset
			mov dword ptr [ebx+2], eax	// смещение начала аргумента functionOffset

			mov eax, pthis
			mov dword ptr [ebx+7], eax	// смещение начала адреса this

			mov eax, dwDWFromStackToCopy
			mov dword ptr [ebx+12], eax	// смещение начала аргумента dwDWFromStackToCopy

			mov eax, pPointer
			mov dword ptr [ebx+17], eax	// смещение начала адреса Call
		}

		return pFn;
	}


	//------------------------------------------------------------------------------
	// Добавить callback, вызывающий P-код по смещению dwOffset и вернуть его ID

	dword CExternalCallManager::RegisterCallback(const dword dwOffset, const dword dwCountOfDWordsToCopyFromStack)
	{
		// Создать функцию
		byte* pFn = GenegateFunction(dwOffset, m_pInterpreter, dwCountOfDWordsToCopyFromStack);

		// Добавить в список зарегистрированных
		m_vctCallbackFunctions.push_back( pFn ); // (byte*)&CallbackTEST

		// Вернуть ее ID (позицию в контейнере)
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
		const dword		argumentCount,		// Количество двойных слов, которые передаются в callback из Win32.
		CInterpreter*	pOriginal,			// Указатель на основной CInterpreter, который формировал данный ExternalCallManager.
		const dword		functionOffset,		// Смещение начала вызываемой функции в байт-коде.
		dword			esp					// Содержимое физического регистра esp, сохранённое перед занесением аргументов в стек.
	)
	{
		CInterpreter interp;
		interp.InitSession(
			pOriginal->m_vmCurrentState.m_vmMemory.m_pBlockBegin,
			pOriginal->m_vmCurrentState.m_vmMemory.m_dwSizeInBytes,
			functionOffset
		);

		// Заполнение стека-сс:
		dword* pStack = (dword*)interp.m_vmCurrentState.m_callStack.GetRoot();

		// Пропускаем адрес возврата
		esp += sizeof(dword) * (argumentCount); // sizeof(dword) * (argumentCount) = размер аргументов в битах

		// Скопируем параметры в стек VM. Адрес возврата не копируется (см.описание xret)
		for (dword i = 0; i < argumentCount; i++)
		{
			dword* pdw = (dword*)( esp - i * sizeof(dword) );
			interp.m_vmCurrentState.m_callStack.PushDWord( *pdw );
		}

		// Передача управления на callback

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
		// Ждем возврата управления...

		// Возвращаем то что вернула команда xret
		return interp.m_vmCurrentState.m_vmProcessor.lock;
	}

	//------------------------------------------------------------------------------

	//------------------------------------------------------------------------------
	// Просто нахождение адреса указанной функции в указанном модуле и сохранение
	// этого адреса в векторе. Возвращает позицию сохраненного адреса в векторе.
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

		const dword dwStackSizeBeforeExternCall = vmState.m_vmProcessor.dwStackSizeBeforeExternCall;	// В байтах.
		const dword dwFullStackSize             = vmState.m_callStack.GetSize();						// Количество байт, реально используемое стеком.
		const dword dwCountOfBytes              = dwFullStackSize - dwStackSizeBeforeExternCall;		// Размер в байтах всего что напихал в стек вызывающий внешнюю функцию код.
		const dword dwCountOfDWORDs             = dwCountOfBytes / 4;									// Размер того же в двойных словах.

		void* p = m_vctExternFunctions[dwID];	// Функция, которую будем вызывать.
		dword dwResult;							// То что она вернёт в регистре eax, вычисляется позже.
		

		/*
		dword i;
		dword d;
		for (i = 0; i < dwCountOfDWORDs; i++)
		{
			d = *(dwInitialStackTop + i);
			__asm push d
		}*/

		// Собственно, вызов:
		const dword* pFirstArgAddrInStack = (dword*)(vmState.m_callStack.GetRoot() + dwStackSizeBeforeExternCall);
		__asm
		{
			// Копируем аргументы из стека виртуальной машины в физический
			xor eax, eax						// i = 0
			loop_beg:
				cmp eax, dwCountOfDWORDs
				je end_loop

				mov  ebx, eax					// \ 
				imul ebx, 4						// - ebx = pFirstArgAddrInStack + i * sizeof(dword*)
				add  ebx, pFirstArgAddrInStack	// /
				mov ebx, [ebx]					// Можно сократить, но это для отладки.
				push ebx						// push( *ebx )

				inc eax							// i++
				jmp loop_beg
			end_loop:

			// Если при регистрации callback'a происходит вылет в этом месте, то вероятно это значит
			// ЧТО НУЖНО ОТКЛЮЧИТЬ ЧЁРТОВ DEP, предотвращающий выполнение данных.
			call p
			mov dwResult, eax

			// Очищать стек НЕ НАДО
		}

		// Восстанавливаем размер стека, который был сохранён перед этим командой xlock
		vmState.m_callStack.SetSize( dwStackSizeBeforeExternCall );

		// Если возвращенное функцией значение требуется
		// вернуть в программу, запишем это значение по залоченному смещению.
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