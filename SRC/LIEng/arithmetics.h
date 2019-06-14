#pragma once
#include "VMState.h"

namespace interpreter {
	//------------------------------------------------------------------------------
	// Арифметические команды
	//------------------------------------------------------------------------------

	void add8mm		(CVMState& vmState);
	void add8mv		(CVMState& vmState);
	void add16mm	(CVMState& vmState);
	void add16mv	(CVMState& vmState);
	void add32mm	(CVMState& vmState);
	void add32mv	(CVMState& vmState);

	void sub8mm		(CVMState& vmState);
	void sub8mv		(CVMState& vmState);
	void sub16mm	(CVMState& vmState);
	void sub16mv	(CVMState& vmState);
	void sub32mm	(CVMState& vmState);
	void sub32mv	(CVMState& vmState);

	void mul8mm		(CVMState& vmState);
	void mul8mv		(CVMState& vmState);
	void mul16mm	(CVMState& vmState);
	void mul16mv	(CVMState& vmState);
	void mul32mm	(CVMState& vmState);
	void mul32mv	(CVMState& vmState);

	void imul8mm	(CVMState& vmState);
	void imul8mv	(CVMState& vmState);
	void imul16mm	(CVMState& vmState);
	void imul16mv	(CVMState& vmState);
	void imul32mm	(CVMState& vmState);
	void imul32mv	(CVMState& vmState);

	void div8mm		(CVMState& vmState);
	void div8mv		(CVMState& vmState);
	void div16mm	(CVMState& vmState);
	void div16mv	(CVMState& vmState);
	void div32mm	(CVMState& vmState);
	void div32mv	(CVMState& vmState);

	void idiv8mm	(CVMState& vmState);
	void idiv8mv	(CVMState& vmState);
	void idiv16mm	(CVMState& vmState);
	void idiv16mv	(CVMState& vmState);
	void idiv32mm	(CVMState& vmState);
	void idiv32mv	(CVMState& vmState);

	void neg8m	(CVMState& vmState);
	void neg16m	(CVMState& vmState);
	void neg32m	(CVMState& vmState);

	//------------------------------------------------------------------------------
}