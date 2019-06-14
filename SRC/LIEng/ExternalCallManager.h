#pragma once

#include <vector>
#include "typedefs.h"

namespace interpreter {

	class CInterpreter;

	// Менеджер внешних вызовов. Отвечает за связь с Windows.
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
		// Реализация внешних вызовов

		// Добавить callback-функцию, которая вызывает P-функцию
		// по смещению dwOffset.
		// Возвращает ID зарегистрированной функции.
		// Должна быть вызывана до Interpretep.Run().
		dword RegisterCallback(const dword dwOffset, const dword dwCountOfDWordsToCopyFromStack);

		// Функция регистрирует внешнюю функцию и возвращает ее ID.
		// В отладочной сборке может возбуждать исключения 0x0101 и 0x0102.
		// Должна быть вызывана до Interpreter.Run().
		dword RegisterExternalFunction(const char* pszModuleName, const char* pszFunctionName);

		// Получение физического адреса callback'a по его ID.
		// Используется в команде lea.
		dword GetCallbackAddress(const dword dwID);

		// Вызывает зарегистрированную внешнюю функцию по ее ID.
		// (в т.ч. правильно заполняет стек)
		// Используется в команде xcall.
		void CallExternalFunction(const dword dwID);

	public:
		// Асинхронный вызов P-функции со смещением dwOffset.
		// Вызывается из функций-callback'ов.
		dword Call(
			const dword dwCountOfDWordsToCopyFromStack,
			CInterpreter* pThis,
			const dword dwOffset,
			dword esp
		);

	private:
		// Для предотвращения копирования в callback'ах векторов ссылок
		// (иначе callback не сможет пользоваться внешними функциями),
		// они сделаны статическими.
		static std::vector<void*> m_vctExternFunctions;
		static std::vector<byte*> m_vctCallbackFunctions;
		CInterpreter* m_pInterpreter;
	};

}
