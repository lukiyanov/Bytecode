#pragma once
#include "typedefs.h"
#include "Exceptions.h"
#include <memory.h>

namespace interpreter {

//------------------------------------------------------------------------------

	// ������� ��������� � ������� �������������� ������ ����� � ���,
	// ��� ������ ������������ ���� std ������� ��������������� �
	// ������� ���������. ��� ����� �� ����������� ���� � �������,
	// � ������� ����������.

	// ��� ��� ������� � #pragma deprecated (std::vector),
	// ���������� ������� ���������� ������:
	//
	// AHTUNG!!!
	// ���������� ����� ����� ���������� ��� ��������� ��������������.
	// �� ������� ������������ std, ��-��������, �������� ����������.
	//
	// ������ ������� �� ���������� P-��������� � ���������:
	//		push32v 0x12345678
	//		pop32m  0x00000000
	//		halt
	// Debug:   162000-197000 (228497-������� ��������, 1 ���)
	// Release: 23000-30000   (182518-������� ��������, 1 ���)
	//
	// ������������ ���������� � ������������ Release
	// ������ ������������ � ��������� ����� (2000-5000).

//------------------------------------------------------------------------------
	class CStackOfBytes
	{
	private:
		static const dword sc_dwInitialStackSize = 256;		// ��������� ������ �����
		static const dword sc_dwStackSizeMultiplier = 2;	// ��������� ��� ����� �����

		dword m_dwAllocSize;	// ���������� ��������� ���������� ������ ����
		dword m_dwUsedSize;		// ���������� ����, ������� ������������ ������
		byte* m_pRoot;			// ������ ������� ������, ���������� ��� ����

	public:

		CStackOfBytes(void);
		~CStackOfBytes(void);

		inline void PushByte(const byte value);
		inline byte PopByte();

		inline void PushWord(const word value);
		inline word PopWord();

		inline void  PushDWord(const dword value);
		inline dword PopDWord();

		// ��������� ������ ����� �� wByteCount ����
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
		// ��������, �������� �����������
		//inline void  PushBytes( byte* pMemBlock, const word wByteCount );
		//inline byte* PopBytes( const word wByteCount );
	};

//------------------------------------------------------------------------------
// �������� ����
inline void CStackOfBytes::Clear()
{
	delete[] m_pRoot;

	m_pRoot = new byte[ sc_dwInitialStackSize ];
	m_dwAllocSize = sc_dwInitialStackSize;
	m_dwUsedSize = 0;
}

//------------------------------------------------------------------------------
// ��������� ������ �����, ����� � ���� ������ ��� wByteCount ����.
inline void CStackOfBytes::PrepareToGrow( const word wByteCount )
{
	// ������� ������ ���������� ��� �������� ��������
	// ����������� ����� + wByteCount ����.
	const dword dwNeedleSize = m_dwUsedSize + wByteCount;

	// ���� ����� ��� ��� wByteCount ������ �� ������...
	if ( m_dwAllocSize < dwNeedleSize )
	{
		// ------------------------------------------
		// ���� ���� ���������� ������ �� ����,
		// ��� �� ����� ����������� ������ �����,
		// ������� ��� ������������������ �� ��������.
		// ------------------------------------------

		// ���������, ������� ��� ����� �������� ������.
		dword dwNewAllocatedSize = m_dwAllocSize * sc_dwStackSizeMultiplier;
		while ( dwNewAllocatedSize < dwNeedleSize )
		{
			dwNewAllocatedSize *= sc_dwStackSizeMultiplier;
		}

		// ������� ����� ���� ������, ��������� � ���� ������
		// �� ��������, ����� ���� ��������� ������� ���� ������.
		byte* pNewRoot = new byte[ dwNewAllocatedSize ];
		memcpy( pNewRoot, m_pRoot, m_dwUsedSize );
		delete[] m_pRoot;

		// ������.
		m_pRoot = pNewRoot;
		m_dwAllocSize = dwNewAllocatedSize;
	}
}



//------------------------------------------------------------------------------
// BYTE
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// �������� � ���� 1 ����
inline void CStackOfBytes::PushByte(const byte value)
{
	PrepareToGrow( 1 );
	m_pRoot[ m_dwUsedSize++ ] = value;
}

//------------------------------------------------------------------------------
// ����� �� ����� 1 ����
inline byte CStackOfBytes::PopByte()
{
#ifdef ALLOW_CHECKS
	// ����� ����� ������, ��� ���� � �����?
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
// �������� � ���� 1 �����
inline void CStackOfBytes::PushWord(const word value)
{
	PrepareToGrow( sizeof(value) );

	word* pValue = (word*)(m_pRoot + m_dwUsedSize);
	*pValue = value;

	m_dwUsedSize += sizeof(value);
}

//------------------------------------------------------------------------------
// ����� �� ����� 1 �����
inline word CStackOfBytes::PopWord()
{
	const dword c_dwOpSize = sizeof(word);
#ifdef ALLOW_CHECKS
	// ����� ����� ������, ��� ���� � �����?
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
// �������� � ���� 1 ������� �����
inline void CStackOfBytes::PushDWord(const dword value)
{
	PrepareToGrow( sizeof(value) );

	dword* pValue = (dword*)(m_pRoot + m_dwUsedSize);
	*pValue = value;

	m_dwUsedSize += sizeof(value);
}

//------------------------------------------------------------------------------
// ����� �� ����� 1 ������� �����
inline dword CStackOfBytes::PopDWord()
{
	const dword c_dwOpSize = sizeof(dword);
#ifdef ALLOW_CHECKS
	// ����� ����� ������, ��� ���� � �����?
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
// ��������� ������ ����� �� wByteCount ����
inline void CStackOfBytes::SPop(const word wByteCount)
{
#ifdef ALLOW_CHECKS
	// ����� ����� ������, ��� ���� � �����?
	if ( m_dwUsedSize < wByteCount )
	{
		throw CInterpretException_StackIsEnded( wByteCount, m_dwUsedSize );
	}
#endif
	m_dwUsedSize -= wByteCount;
}

//------------------------------------------------------------------------------
// �������� � ���� wByteCount ����
//inline void CStackOfBytes::PushBytes( byte* pMemBlock, const word wByteCount )
//{
//	PrepareToGrow( wByteCount );
//	memcpy( m_pRoot + m_dwUsedSize, pMemBlock, wByteCount );
//}

//------------------------------------------------------------------------------
// ����� �� ����� wByteCount ����
//inline byte* CStackOfBytes::PopBytes( const word wByteCount )
//{
//#ifdef ALLOW_CHECKS
//	// ����� ����� ������, ��� ���� � �����?
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