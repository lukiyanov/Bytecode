#include "arithmetics.h"
#include "macrogen.h"

namespace interpreter
{
	// ADD
	template <typename T>
	inline void addT ( T* opDest, const T opSrc )
	{
		*opDest += opSrc;
	}

	// SUB
	template <typename T>
	inline void subT ( T* opDest, const T opSrc )
	{
		*opDest -= opSrc;
	}

	// MUL
	template <typename T>
	inline void mulT ( T* opDest, const T opSrc )
	{
		*opDest *= opSrc;
	}

	// DIV
	template <typename T>
	inline void divT ( T* opDest, const T opSrc )
	{
		*opDest /= opSrc;
	}

	// NEG
	template <typename T>
	inline void negT ( T* param )
	{
		*param = - *param;
	}


#define BYTE  byte
#define WORD  word
#define DWORD dword

	GENERATE_BLOCK_8x16x32_MMxMV( addT,  add  )
	GENERATE_BLOCK_8x16x32_MMxMV( subT,  sub  )
	GENERATE_BLOCK_8x16x32_MMxMV( mulT,  mul  )
	GENERATE_BLOCK_8x16x32_MMxMV( divT,  div  )

#undef BYTE
#undef WORD
#undef DWORD

#define BYTE  sbyte
#define WORD  sword
#define DWORD sdword

	GENERATE_BLOCK_8x16x32_MMxMV( mulT, imul )
	GENERATE_BLOCK_8x16x32_MMxMV( divT, idiv )

	GENERATE_BLOCK_8x16x32_M( negT, neg )

}