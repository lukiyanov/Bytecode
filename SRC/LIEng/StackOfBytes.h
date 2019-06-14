#pragma once
#include "typedefs.h"
#include "Exceptions.h"
#include <memory.h>

namespace interpreter {

//------------------------------------------------------------------------------

	// Причина появления в составе интерпретатора класса стека в том,
	// что классы пространства имен std слишком всеохватывающие и
	// слишком тормозные. Мне нужен не сферический конь в вакууме,
	// а быстрая реализация.

	// Вот моя заметка к #pragma deprecated (std::vector),
	// поясняющая причину применения прагмы:
	//
	// AHTUNG!!!
	// Реализацию стека нужно переделать для повышения быстродействия.
	// От классов пространства std, по-видимому, придется отказаться.
	//
	// Замеры времени на выполнение P-программы с командами:
	//		push32v 0x12345678
	//		pop32m  0x00000000
	//		halt
	// Debug:   162000-197000 (228497-пиковый максимум, 1 раз)
	// Release: 23000-30000   (182518-пиковый максимум, 1 раз)
	//
	// Переделанная реализация в конфигурации Release
	// должна укладываться в несколько тысяч (2000-5000).

//------------------------------------------------------------------------------
	class CStackOfBytes
	{
	private:
		static const dword sc_dwInitialStackSize = 256;		// Начальный размер стека
		static const dword sc_dwStackSizeMultiplier = 2;	// Множитель при росте стека

		dword m_dwAllocSize;	// Количество физически выделенных стеком байт
		dword m_dwUsedSize;		// Количество байт, реально используемое стеком
		byte* m_pRoot;			// Начало области памяти, выделенной под стек

	public:

		CStackOfBytes(void);
		~CStackOfBytes(void);

		inline void PushByte(const byte value);
		inline byte PopByte();

		inline void PushWord(const word value);
		inline word PopWord();

		inline void  PushDWord(const dword value);
		inline dword PopDWord();

		// Уменьшить размер стека на wByteCount байт
		inline void SPop(const word wByteCount);

		inline void Clear();

		inline byte* GetRoot()
		{
			return m_pRoot;
		}

		inline dword GetSize() const
		{
			return m_dwUsedSize;
		}

		inline void SetSize(const dword dwUsedSize)
		{
			m_dwUsedSize = dwUsedSize;
		}

	private:
		inline void PrepareToGrow( const word wByteCount );

	public:
		// Возможно, придется реализовать
		//inline void  PushBytes( byte* pMemBlock, const word wByteCount );
		//inline byte* PopBytes( const word wByteCount );
	};

//------------------------------------------------------------------------------
// Очистить стек
inline void CStackOfBytes::Clear()
{
	delete[] m_pRoot;

	m_pRoot = new byte[ sc_dwInitialStackSize ];
	m_dwAllocSize = sc_dwInitialStackSize;
	m_dwUsedSize = 0;
}

//------------------------------------------------------------------------------
// Увеличить размер стека, чтобы в него влезло еще wByteCount байт.
inline void CStackOfBytes::PrepareToGrow( const word wByteCount )
{
	// Сколько памяти необходимо для хранения прежнего
	// содержимого стека + wByteCount байт.
	const dword dwNeedleSize = m_dwUsedSize + wByteCount;

	// Если места еще под wByteCount байтов не хватит...
	if ( m_dwAllocSize < dwNeedleSize )
	{
		// ------------------------------------------
		// Этот блок кодируется исходя из того,
		// что он будет выполняться крайне редко,
		// поэтому его производительность не критична.
		// ------------------------------------------

		// Установим, сколько нам нужно выделить памяти.
		dword dwNewAllocatedSize = m_dwAllocSize * sc_dwStackSizeMultiplier;
		while ( dwNewAllocatedSize < dwNeedleSize )
		{
			dwNewAllocatedSize *= sc_dwStackSizeMultiplier;
		}

		// Выделим новый блок памяти, скопируем в него данные
		// из прежнего, после чего освободим прежний блок памяти.
		byte* pNewRoot = new byte[ dwNewAllocatedSize ];
		memcpy( pNewRoot, m_pRoot, m_dwUsedSize );
		delete[] m_pRoot;

		// Готово.
		m_pRoot = pNewRoot;
		m_dwAllocSize = dwNewAllocatedSize;
	}
}



//------------------------------------------------------------------------------
// BYTE
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Положить в стек 1 байт
inline void CStackOfBytes::PushByte(const byte value)
{
	PrepareToGrow( 1 );
	m_pRoot[ m_dwUsedSize++ ] = value;
}

//------------------------------------------------------------------------------
// Взять со стека 1 байт
inline byte CStackOfBytes::PopByte()
{
#ifdef ALLOW_CHECKS
	// Хотим взять больше, чем есть в стеке?
	if ( !m_dwUsedSize )
	{
		throw CInterpretException_StackIsEnded( 1, 0 );
	}
#endif

	return m_pRoot[ --m_dwUsedSize ];
}


//------------------------------------------------------------------------------
// WORD
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Положить в стек 1 слово
inline void CStackOfBytes::PushWord(const word value)
{
	PrepareToGrow( sizeof(value) );

	word* pValue = (word*)(m_pRoot + m_dwUsedSize);
	*pValue = value;

	m_dwUsedSize += sizeof(value);
}

//------------------------------------------------------------------------------
// Взять со стека 1 слово
inline word CStackOfBytes::PopWord()
{
	const dword c_dwOpSize = sizeof(word);
#ifdef ALLOW_CHECKS
	// Хотим взять больше, чем есть в стеке?
	if ( m_dwUsedSize < c_dwOpSize )
	{
		throw CInterpretException_StackIsEnded( c_dwOpSize, m_dwUsedSize );
	}
#endif

	word* pValue = (word*)(m_pRoot + m_dwUsedSize - c_dwOpSize);
	m_dwUsedSize -= c_dwOpSize;

	return *pValue;
}



//------------------------------------------------------------------------------
// DWORD
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Положить в стек 1 двойное слово
inline void CStackOfBytes::PushDWord(const dword value)
{
	PrepareToGrow( sizeof(value) );

	dword* pValue = (dword*)(m_pRoot + m_dwUsedSize);
	*pValue = value;

	m_dwUsedSize += sizeof(value);
}

//------------------------------------------------------------------------------
// Взять со стека 1 двойное слово
inline dword CStackOfBytes::PopDWord()
{
	const dword c_dwOpSize = sizeof(dword);
#ifdef ALLOW_CHECKS
	// Хотим взять больше, чем есть в стеке?
	if ( m_dwUsedSize < c_dwOpSize )
	{
		throw CInterpretException_StackIsEnded( c_dwOpSize, m_dwUsedSize );
	}
#endif

	dword* pValue = (dword*)(m_pRoot + m_dwUsedSize - c_dwOpSize);
	m_dwUsedSize -= c_dwOpSize;

	return *pValue;
}



//------------------------------------------------------------------------------
// Уменьшить размер стека на wByteCount байт
inline void CStackOfBytes::SPop(const word wByteCount)
{
#ifdef ALLOW_CHECKS
	// Хотим взять больше, чем есть в стеке?
	if ( m_dwUsedSize < wByteCount )
	{
		throw CInterpretException_StackIsEnded( wByteCount, m_dwUsedSize );
	}
#endif
	m_dwUsedSize -= wByteCount;
}

//------------------------------------------------------------------------------
// Положить в стек wByteCount байт
//inline void CStackOfBytes::PushBytes( byte* pMemBlock, const word wByteCount )
//{
//	PrepareToGrow( wByteCount );
//	memcpy( m_pRoot + m_dwUsedSize, pMemBlock, wByteCount );
//}

//------------------------------------------------------------------------------
// Взять со стека wByteCount байт
//inline byte* CStackOfBytes::PopBytes( const word wByteCount )
//{
//#ifdef ALLOW_CHECKS
//	// Хотим взять больше, чем есть в стеке?
//	if ( m_dwUsedSize < wByteCount )
//	{
//		throw CInterpretException_StackIsEnded( wByteCount, m_dwUsedSize );
//	}
//#endif
//
//	// NOT DONE, MAYBE TODO
//}

//------------------------------------------------------------------------------

}