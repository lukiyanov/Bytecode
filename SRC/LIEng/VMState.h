#pragma once
#include "typedefs.h"
#include "Exceptions.h"
#include "StackOfBytes.h"

namespace interpreter {

	class CExternalCallManager;

////////////////////////////////////////////////////////////////////////////////
//					Текущее состояние процессора
////////////////////////////////////////////////////////////////////////////////

	struct CVMRegisters
	{
		// any
		// Смещение текущей команды
		dword eip;

		// cmpxxx
		// Истинность последнего заданного условия
		bool bct;	// Condition is true

		// lock-ret
		// Смещение, по которому записывать результат функции.
		// Кроме того, в случае внешнего callback'а сюда заносится
		// возвращаемое значение перед завершением callback'а.
		dword lock;

		// extern
		dword dwStackSizeBeforeExternCall;

	public:
		// init
		inline void Clear(const dword ip)
		{
			eip  = ip;
			bct  = 0;
			lock = 0;

			dwStackSizeBeforeExternCall = 0;
		}

	};

////////////////////////////////////////////////////////////////////////////////
//					Текущее состояние памяти
////////////////////////////////////////////////////////////////////////////////

	struct CFlatMemoryBlockOrganization
	{
		byte* m_pBlockBegin;
		dword m_dwSizeInBytes;

	public:
		void Init(byte* pCode, const dword dwSizeOfCode)
		{
			m_pBlockBegin = pCode;
			m_dwSizeInBytes = dwSizeOfCode;
		}
	};

	typedef CFlatMemoryBlockOrganization CVMMemoryBlock;

////////////////////////////////////////////////////////////////////////////////
//					Текущее состояние системы
////////////////////////////////////////////////////////////////////////////////

	struct CVMState
	{
		// Реализация регистров процессора
		CVMRegisters   m_vmProcessor;
		// Реализация памяти
		CVMMemoryBlock m_vmMemory;
		// Реализация стека
		CStackOfBytes  m_callStack;
		// Менеджер внешних вызовов
		CExternalCallManager* m_pExternalCallManager;

		bool m_bStillWorking;

	public:
		//------------------------------------------------------------------------------
		// Инициализация
		void Init(byte* pCode, const dword dwSizeOfCode, const dword dwEntryPoint, CExternalCallManager* pExternalCallManager)
		{
			m_bStillWorking = 1;

			m_vmMemory.Init( pCode, dwSizeOfCode );
			m_vmProcessor.Clear( dwEntryPoint );
//			m_callStack.Clear();

			m_pExternalCallManager = pExternalCallManager;
		}

		//------------------------------------------------------------------------------
		// Получить реальный адрес первого байта за концом опкода текущей команды
		inline dword GetVA() const;

		//------------------------------------------------------------------------------
		// Проверить блок памяти начиная с eip и заканчивая
		// (eip + dwByteCount - 1) на доступность для чтения.
		// 1. Для повышения производительности проверка включена только в отладочной версии
		// 2. Вызов "CheckBytes(0)" бессмысленен даже в отладочной версии.
		__forceinline void CheckBytes (const dword dwByteCount) const;

		//------------------------------------------------------------------------------
		// Проверить блок памяти начиная с eip и заканчивая
		// (eip + dwByteCount - 1) на доступность для чтения.
		__forceinline void CheckMemoryAt (const dword dwBlockBegin, const dword dwByteCount);

		//------------------------------------------------------------------------------
		inline dword GetVaFromOffset (const dword offset);

		//------------------------------------------------------------------------------
		inline byte*  Read_8m  ();
		inline word*  Read_16m ();
		inline dword* Read_32m ();

		//------------------------------------------------------------------------------
		inline const byte  Read_8v  ();
		inline const word  Read_16v ();
		inline const dword Read_32v ();

		//------------------------------------------------------------------------------
		// Работа со стеком
		inline void StackPushB (const byte value);
		inline void StackPushW (const word value);
		inline void StackPushD (const dword value);
		inline byte  StackPopB ();
		inline word  StackPopW ();
		inline dword StackPopD ();
		inline void SStackPop (const word count);

		//------------------------------------------------------------------------------
		inline void JumpTo (const dword dwOffset);


	public:
	};

	typedef CVMState CVMState;

////////////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------------
inline dword CVMState::GetVA() const
{
	return (dword)m_vmMemory.m_pBlockBegin + m_vmProcessor.eip;
}

//------------------------------------------------------------------------------
__forceinline void CVMState::CheckBytes (const dword dwByteCount) const
{
#ifdef ALLOW_CHECKS

	// Смещение последнего из запрашиваемых байтов...
	const dword dwLastRequestedByte = m_vmProcessor.eip + dwByteCount - 1;

	// ... должно быть меньше, чем размер выделенного АП...
	if ( dwLastRequestedByte >= m_vmMemory.m_dwSizeInBytes )
	{
		// ... если это не так - найдем смещение первого байта, выходящего за пределы АП.
		for (dword i = m_vmProcessor.eip; i <= dwLastRequestedByte; i++)
		{
			if ( i >= m_vmMemory.m_dwSizeInBytes )
				throw CInterpretException_ReferenceIsOutOfAllocatedMemory( i, trRead );
		}
	}
#endif
}

//------------------------------------------------------------------------------
__forceinline void CVMState::CheckMemoryAt (const dword dwBlockBegin, const dword dwByteCount)
{
#ifdef ALLOW_CHECKS

	// Смещение последнего из запрашиваемых байтов...
	const dword dwLastRequestedByte = dwBlockBegin + dwByteCount - 1;

	// ... должно быть меньше, чем размер выделенного АП...
	if ( dwLastRequestedByte >= m_vmMemory.m_dwSizeInBytes )
	{
		// ... если это не так - найдем смещение первого байта, выходящего за пределы АП.
		for (dword i = dwBlockBegin; i <= dwLastRequestedByte; i++)
		{
			if ( i >= m_vmMemory.m_dwSizeInBytes )
				throw CInterpretException_ReferenceIsOutOfAllocatedMemory( i, trRead );
		}
	}
#endif
}

//------------------------------------------------------------------------------
inline dword CVMState::GetVaFromOffset (const dword offset)
{
	const dword dwResult = (dword)m_vmMemory.m_pBlockBegin + offset;
	return dwResult;
}

//------------------------------------------------------------------------------
inline byte*  CVMState::Read_8m ()
{
	const byte nBytesToReed = 4;
	CheckBytes( nBytesToReed );

	const dword ref = *( (dword*) GetVA() );
	CheckMemoryAt( ref, sizeof(byte) );

	byte* refToVar = (byte*)(
			(dword) m_vmMemory.m_pBlockBegin + ref
		);

	m_vmProcessor.eip += nBytesToReed;

	return refToVar;
}

//------------------------------------------------------------------------------
inline word*  CVMState::Read_16m ()
{
	const byte nBytesToReed = 4;
	CheckBytes( nBytesToReed );

	const dword ref = *( (dword*) GetVA() );
	CheckMemoryAt( ref, sizeof(word) );

	word* refToVar = (word*)(
			(dword) m_vmMemory.m_pBlockBegin + ref
		);

	m_vmProcessor.eip += nBytesToReed;

	return refToVar;
}

//------------------------------------------------------------------------------
inline dword* CVMState::Read_32m ()
{
	const byte nBytesToReed = 4;
	CheckBytes( nBytesToReed );

	const dword ref = *( (dword*) GetVA() );
	CheckMemoryAt( ref, sizeof(dword) );

	dword* refToVar = (dword*)(
			(dword) m_vmMemory.m_pBlockBegin + ref
		);

	m_vmProcessor.eip += nBytesToReed;

	return refToVar;
}

//------------------------------------------------------------------------------
inline const byte  CVMState::Read_8v ()
{
	const byte nBytesToReed = 1;
	CheckBytes( nBytesToReed );

	const byte value = *( (byte*) GetVA() );

	m_vmProcessor.eip += nBytesToReed;

	return value;
}

//------------------------------------------------------------------------------
inline const word  CVMState::Read_16v ()
{
	const byte nBytesToReed = 2;
	CheckBytes( nBytesToReed );

	const word value = *( (word*) GetVA() );

	m_vmProcessor.eip += nBytesToReed;

	return value;
}

//------------------------------------------------------------------------------
inline const dword CVMState::Read_32v ()
{
	const byte nBytesToReed = 4;
	CheckBytes( nBytesToReed );

	const dword value = *( (dword*) GetVA() );

	m_vmProcessor.eip += nBytesToReed;

	return value;
}

// PUSH
//------------------------------------------------------------------------------
inline void CVMState::StackPushB (const byte value)
{
	m_callStack.PushByte( value );
}

//------------------------------------------------------------------------------
inline void CVMState::StackPushW (const word value)
{
	m_callStack.PushWord( value );
}

//------------------------------------------------------------------------------
inline void CVMState::StackPushD (const dword value)
{
	m_callStack.PushDWord( value );
}

// POP
//------------------------------------------------------------------------------
inline byte CVMState::StackPopB ()
{
	return 	m_callStack.PopByte();
}

//------------------------------------------------------------------------------
inline word CVMState::StackPopW ()
{
	return 	m_callStack.PopWord();
}

//------------------------------------------------------------------------------
inline dword CVMState::StackPopD ()
{
	return 	m_callStack.PopDWord();
}

// SPOP
//------------------------------------------------------------------------------
inline void CVMState::SStackPop (const word count)
{
	m_callStack.SPop( count );
}

// JMP
//------------------------------------------------------------------------------
inline void CVMState::JumpTo (const dword dwOffset)
{
	CheckMemoryAt( dwOffset, sizeof(opcode) );
	m_vmProcessor.eip = dwOffset;
}

//------------------------------------------------------------------------------

} // namespace interpreter
