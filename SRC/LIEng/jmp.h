#pragma once

#include "VMState.h"

namespace interpreter {
	//------------------------------------------------------------------------------
	// JMP
	void jmp32m	(CVMState& vmState);
	void jmp32v	(CVMState& vmState);

	//------------------------------------------------------------------------------
}