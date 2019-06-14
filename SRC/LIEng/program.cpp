#include "Interpreter.h"

InterpreterEngine_1_by_dcLout::byte aCode[] = {
111, 0, 94, 0, 0, 0, -117, 0,
0, 0, 99, 0, 94, 0, 0, 0,
19, 0, 0, 0, 0, 0, 22, 0,
94, 0, 0, 0, 22, 0, 94, 0,
0, 0, 19, 0, 0, 0, 0, 0,
100, 0, 0, 0, 0, 0, 111, 0,
-105, 0, 0, 0, -99, 0, 0, 0,
-127, 0, -105, 0, 0, 0, 5, 0,
0, 0, 113, 0, -101, 0, 0, 0,
-105, 0, 0, 0, -5, -1, 5, 0,
-101, 0, 0, 0, -101, 0, 0, 0,
10, 0, 98, 0, 0, 0, 0, 0,
0, 0, 124, 0, -125, 0, 0, 0,
-126, 0, 0, 0, 12, 0, -125, 0,
0, 0, -125, 0, 0, 0, 7, 0,
94, 0, 0, 0, -125, 0, 0, 0,
1, 0, 2, 3, 0, 0, 0, 120,
86, 52, 18, 72, 101, 108, 108, 111,
32, 119, 111, 114, 108, 100, 0, 0,
0, 0, 0, 0, 0, -38, 0
};

void SetInterpreterExtern( InterpreterEngine_1_by_dcLout::CInterpreter& InterASM )
{
	InterASM.RegisterExternalFunction( "user32", "MessageBoxA" );

}