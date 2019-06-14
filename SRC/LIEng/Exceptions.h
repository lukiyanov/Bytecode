#pragma once
#include "typedefs.h"
#include <string>

namespace interpreter {

#ifdef ALLOW_CHECKS
	//------------------------------------------------------------------------------
	// Используется в CInterpretException_ReferenceIsOutOfAllocatedMemory для
	// указания типа обращения к несуществующей ячейке
	enum ETreatReason
	{				// Предполагалось, что ячейка:
		trRead,		// данные, которые требовалось прочитать
		trWrite,	// данные, которые требовалось записать
		trExecute	// код операции
	};

	typedef unsigned __int64 RVA;

	////////////////////////////////////////////////////////////////////////////////
	//								Исключения
	////////////////////////////////////////////////////////////////////////////////

	//------------------------------------------------------------------------------
	// Обобщенная исключительная ситуация
	struct CInterpretException
	{
		virtual std::string Message() = 0;
		virtual ~CInterpretException(){}
	};

	//------------------------------------------------------------------------------
	// Встречен неизвестный код операции
	struct CInterpretException_UnknownCommand
		: public CInterpretException
	{
		RVA  m_rvaOffset;	// Смещение команды от базового адреса загрузки P-кода
		opcode m_opCode;	// Неопознанная команда

	public:
		CInterpretException_UnknownCommand
		(
			const RVA rvaOffset, const opcode opCode
		):
		m_rvaOffset(rvaOffset), m_opCode(opCode) {}

		std::string Message();
	};

	//------------------------------------------------------------------------------
	// Попытка обращения к несуществующему участку памяти виртуальной машины
	struct CInterpretException_ReferenceIsOutOfAllocatedMemory
		: public CInterpretException
	{
		RVA  m_rvaOffset;		// Смещение команды от базового адреса загрузки P-кода
		ETreatReason m_tReason;	// Зачем предполагалось использовать ячейку памяти

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
	// Попытка POP'нуть из стека больше, чем в нем есть
	struct CInterpretException_StackIsEnded
		: public CInterpretException
	{
		dword m_wdCount;		// Количество байт, которое хотели POP'нуть
		dword m_dwStackSize;	// Количество байт, которое было в стеке

	public:
		CInterpretException_StackIsEnded
		(
			const dword wdCount, const dword dwStackSize
		):
		m_wdCount(wdCount), m_dwStackSize(dwStackSize) {}

		std::string Message();
	};

	//------------------------------------------------------------------------------
	// Попытка получить адрес незарегистрированного callback'а
	struct CInterpretException_UnknownCallbackID
		: public CInterpretException
	{
		dword m_dwMaxID;	// Максимально допустимый ID
		dword m_dwFoundID;	// Обнаруженный ID

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