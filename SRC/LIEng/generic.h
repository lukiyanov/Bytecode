#pragma once
#include "VMState.h"

namespace interpreter {
	//------------------------------------------------------------------------------
	// Базовые команды
	void nop		(CVMState& vmState);
	void halt		(CVMState& vmState);
	void brk		(CVMState& vmState);
	//------------------------------------------------------------------------------
}