#pragma once
#include "VMState.h"

namespace interpreter {
	//------------------------------------------------------------------------------
	// –абота с указател€ми
	//------------------------------------------------------------------------------

	// m = *(p+o), o - offset, int, signed
	// mov var1, [var2]
	void mov8mpo		(CVMState& vmState);
	void mov16mpo		(CVMState& vmState);
	void mov32mpo		(CVMState& vmState);

	// *(p+o) = m
	// mov dword [var2], var1 // ptr не нужен
	void mov8pom		(CVMState& vmState);
	void mov16pom		(CVMState& vmState);
	void mov32pom		(CVMState& vmState);

	// *(p+o) = v
	// mov dword [var2], 2 // ptr не нужен
	void mov8pov		(CVMState& vmState);
	void mov16pov		(CVMState& vmState);
	void mov32pov		(CVMState& vmState);

	// *(p1+o1) = *(p2+o2)
	// mov dword [var1], [var2] // ptr не нужен
	void mov8popo		(CVMState& vmState);
	void mov16popo		(CVMState& vmState);
	void mov32popo		(CVMState& vmState);
	//------------------------------------------------------------------------------
}