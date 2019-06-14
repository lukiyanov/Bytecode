#pragma once

// functionTemplate( T* pSrc, const T tDst);

// ----------------- xMM -----------------
#define GENERATE_CMD_8MM( functionTemplate, cmdBaseName )\
void cmdBaseName##8mm (CVMState& vmState)\
{\
	BYTE* memDst = (BYTE*)vmState.Read_8m();\
	BYTE* memSrc = (BYTE*)vmState.Read_8m();\
	functionTemplate( memDst, *memSrc );\
}

#define GENERATE_CMD_16MM( functionTemplate, cmdBaseName )\
void cmdBaseName##16mm (CVMState& vmState)\
{\
	WORD* memDst = (WORD*)vmState.Read_16m();\
	WORD* memSrc = (WORD*)vmState.Read_16m();\
	functionTemplate( memDst, *memSrc );\
}

#define GENERATE_CMD_32MM( functionTemplate, cmdBaseName )\
void cmdBaseName##32mm (CVMState& vmState)\
{\
	DWORD* memDst = (DWORD*)vmState.Read_32m();\
	DWORD* memSrc = (DWORD*)vmState.Read_32m();\
	functionTemplate( memDst, *memSrc );\
}

// ----------------- xMV -----------------
#define GENERATE_CMD_8MV( functionTemplate, cmdBaseName )\
void cmdBaseName##8mv (CVMState& vmState)\
{\
	BYTE* memDst = (BYTE*)vmState.Read_8m();\
	const BYTE valSrc = (BYTE)vmState.Read_8v();\
	functionTemplate( memDst, valSrc );\
}

#define GENERATE_CMD_16MV( functionTemplate, cmdBaseName )\
void cmdBaseName##16mv (CVMState& vmState)\
{\
	WORD* memDst = (WORD*)vmState.Read_16m();\
	const WORD valSrc = (WORD)vmState.Read_16v();\
	functionTemplate( memDst, valSrc );\
}

#define GENERATE_CMD_32MV( functionTemplate, cmdBaseName )\
void cmdBaseName##32mv (CVMState& vmState)\
{\
	DWORD* memDst = (DWORD*)vmState.Read_32m();\
	const DWORD valSrc = (DWORD)vmState.Read_32v();\
	functionTemplate( memDst, valSrc );\
}

// ----------------- BLOCK -----------------
#define GENERATE_BLOCK_8x16x32_MMxMV( functionTemplate, cmdBaseName )\
	GENERATE_CMD_8MM(  functionTemplate, cmdBaseName )\
	GENERATE_CMD_8MV(  functionTemplate, cmdBaseName )\
	GENERATE_CMD_16MM( functionTemplate, cmdBaseName )\
	GENERATE_CMD_16MV( functionTemplate, cmdBaseName )\
	GENERATE_CMD_32MM( functionTemplate, cmdBaseName )\
	GENERATE_CMD_32MV( functionTemplate, cmdBaseName )

//------------------------------------------------------------------------------
#define GENERATE_CMD_8M( functionTemplate, cmdBaseName )\
void cmdBaseName##8m (CVMState& vmState)\
{\
	BYTE* param = (BYTE*)vmState.Read_8m();\
	functionTemplate( param );\
}

#define GENERATE_CMD_16M( functionTemplate, cmdBaseName )\
void cmdBaseName##16m (CVMState& vmState)\
{\
	WORD* param = (WORD*)vmState.Read_16m();\
	functionTemplate( param );\
}

#define GENERATE_CMD_32M( functionTemplate, cmdBaseName )\
void cmdBaseName##32m (CVMState& vmState)\
{\
	DWORD* param = (DWORD*)vmState.Read_32m();\
	functionTemplate( param );\
}

// ----------------- BLOCK -----------------
#define GENERATE_BLOCK_8x16x32_M( functionTemplate, cmdBaseName )\
	GENERATE_CMD_8M(  functionTemplate, cmdBaseName )\
	GENERATE_CMD_16M( functionTemplate, cmdBaseName )\
	GENERATE_CMD_32M( functionTemplate, cmdBaseName )
