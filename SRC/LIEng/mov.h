#pragma once
#include "VMState.h"

namespace interpreter {
	//------------------------------------------------------------------------------
	// MOV
	void mov8mm		(CVMState& vmState);
	void mov8mv		(CVMState& vmState);
	void mov16mm	(CVMState& vmState);
	void mov16mv	(CVMState& vmState);
	void mov32mm	(CVMState& vmState);
	void mov32mv	(CVMState& vmState);

	//------------------------------------------------------------------------------
}
