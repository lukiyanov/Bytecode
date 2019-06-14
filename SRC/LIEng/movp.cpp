#include "movp.h"

namespace interpreter {

	inline void* GetPointer ( dword *pSrc, const word w )
	{
		dword dwVA = *pSrc;
		const sword offset = ( sword ) w;
		dwVA += offset;

		return reinterpret_cast< void* >( dwVA );
	}

	//------------------------------------------------------------------------------
	// m = *(p+o), o - offset, int, signed
	// mov var1, [var2]
	void mov8mpo( CVMState& vmState )
	{
		byte  *pDst = vmState.Read_8m();
		dword *pSrc = vmState.Read_32m();
		const word offset = vmState.Read_16v();

		*pDst = *( (byte*) GetPointer( pSrc, offset ) );
	}

	void mov16mpo( CVMState& vmState )
	{
		word  *pDst = vmState.Read_16m();
		dword *pSrc = vmState.Read_32m();
		const word offset = vmState.Read_16v();

		*pDst = *( (word*) GetPointer( pSrc, offset ) );
	}

	void mov32mpo( CVMState& vmState )
	{
		dword *pDst = vmState.Read_32m();
		dword *pSrc = vmState.Read_32m();
		const word offset = vmState.Read_16v();

		*pDst = *( (dword*) GetPointer( pSrc, offset ) );
	}


	//------------------------------------------------------------------------------
	// *(p+o) = m
	// mov dword [var2], var1 // ptr не нужен
	void mov8pom( CVMState& vmState )
	{
		dword *pSrc = vmState.Read_32m();
		const word offset = vmState.Read_16v();
		byte  *pDst = vmState.Read_8m();

		*( (byte*) GetPointer( pSrc, offset ) ) = *pDst;
	}

	void mov16pom( CVMState& vmState )
	{
		dword *pSrc = vmState.Read_32m();
		const word offset = vmState.Read_16v();
		word  *pDst = vmState.Read_16m();

		*( (word*) GetPointer( pSrc, offset ) ) = *pDst;
	}

	void mov32pom( CVMState& vmState )
	{
		dword *pSrc = vmState.Read_32m();
		const word offset = vmState.Read_16v();
		dword *pDst = vmState.Read_32m();

		*( (dword*) GetPointer( pSrc, offset ) ) = *pDst;
	}


	//------------------------------------------------------------------------------
	// *(p+o) = v
	// mov dword [var2], 2 // ptr не нужен
	void mov8pov( CVMState& vmState )
	{
		dword *pSrc = vmState.Read_32m();
		const word offset = vmState.Read_16v();
		const byte value = vmState.Read_8v();

		*( (byte*) GetPointer( pSrc, offset ) ) = value;
	}

	void mov16pov( CVMState& vmState )
	{
		dword *pSrc = vmState.Read_32m();
		const word offset = vmState.Read_16v();
		const word value = vmState.Read_16v();

		*( (word*) GetPointer( pSrc, offset ) ) = value;
	}

	void mov32pov( CVMState& vmState )
	{
		dword *pSrc = vmState.Read_32m();
		const word offset = vmState.Read_16v();
		const dword value = vmState.Read_32v();

		*( (dword*) GetPointer( pSrc, offset ) ) = value;
	}


	//------------------------------------------------------------------------------
	// *(p1+o1) = *(p2+o2)
	// mov dword [var1], [var2] // ptr не нужен
	void mov8popo( CVMState& vmState )
	{
		dword *ptr1 = vmState.Read_32m();
		const word offset1 = vmState.Read_16v();
		dword *ptr2 = vmState.Read_32m();
		const word offset2 = vmState.Read_16v();

		*( (byte*) GetPointer( ptr1, offset1 ) ) = *( (byte*) GetPointer( ptr2, offset2 ) );
	}

	void mov16popo( CVMState& vmState )
	{
		dword *ptr1 = vmState.Read_32m();
		const word offset1 = vmState.Read_16v();
		dword *ptr2 = vmState.Read_32m();
		const word offset2 = vmState.Read_16v();

		*( (word*) GetPointer( ptr1, offset1 ) ) = *( (word*) GetPointer( ptr2, offset2 ) );
	}

	void mov32popo( CVMState& vmState )
	{
		dword *ptr1 = vmState.Read_32m();
		const word offset1 = vmState.Read_16v();
		dword *ptr2 = vmState.Read_32m();
		const word offset2 = vmState.Read_16v();

		*( (dword*) GetPointer( ptr1, offset1 ) ) = *( (dword*) GetPointer( ptr2, offset2 ) );
	}

	//------------------------------------------------------------------------------

}