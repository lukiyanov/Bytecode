#pragma once
#include "VMState.h"

namespace interpreter {
	//------------------------------------------------------------------------------
	// Логические команды
	//------------------------------------------------------------------------------
	void and8mm	 (CVMState& vmState);
	void and8mv	 (CVMState& vmState);
	void and16mm (CVMState& vmState);
	void and16mv (CVMState& vmState);
	void and32mm (CVMState& vmState);
	void and32mv (CVMState& vmState);

	void or8mm	(CVMState& vmState);
	void or8mv	(CVMState& vmState);
	void or16mm	(CVMState& vmState);
	void or16mv	(CVMState& vmState);
	void or32mm	(CVMState& vmState);
	void or32mv	(CVMState& vmState);

	void xor8mm	 (CVMState& vmState);
	void xor8mv	 (CVMState& vmState);
	void xor16mm (CVMState& vmState);
	void xor16mv (CVMState& vmState);
	void xor32mm (CVMState& vmState);
	void xor32mv (CVMState& vmState);

	void not8m	 (CVMState& vmState);
	void not16m	 (CVMState& vmState);
	void not32m	 (CVMState& vmState);
}