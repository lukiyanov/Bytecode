#pragma once
#include "VMState.h"

namespace interpreter {
	//------------------------------------------------------------------------------
	// ������� �������
	void nop		(CVMState& vmState);
	void halt		(CVMState& vmState);
	void brk		(CVMState& vmState);
	//------------------------------------------------------------------------------
}