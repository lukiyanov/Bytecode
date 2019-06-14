#include "logical.h"
#include "macrogen.h"

namespace interpreter
{
	// AND
	template <typename T>
	inline void andT ( T* opDest, const T opSrc )
	{
		*opDest &= opSrc;
	}

	// OR
	template <typename T>
	inline void orT ( T* opDest, const T opSrc )
	{
		*opDest |= opSrc;
	}

	// XOR
	template <typename T>
	inline void xorT ( T* opDest, const T opSrc )
	{
		*opDest ^= opSrc;
	}

	// NOT
	template <typename T>
	inline void notT ( T* opDest )
	{
		*opDest = ~*opDest;
	}

#define BYTE  byte
#define WORD  word
#define DWORD dword

	GENERATE_BLOCK_8x16x32_MMxMV( andT, and )
	GENERATE_BLOCK_8x16x32_MMxMV( orT,  or  )
	GENERATE_BLOCK_8x16x32_MMxMV( xorT, xor )
	GENERATE_BLOCK_8x16x32_M( notT, not )
}