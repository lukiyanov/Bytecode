#pragma once
#include "VMState.h"

namespace interpreter {

	// XLOCK
 	void xlock  (CVMState& vmState);
 	void xlockm (CVMState& vmState);

	// XCALL
	void xcall32v (CVMState& vmState);
	void xcall32m (CVMState& vmState);

	// XRET
	void xret    (CVMState& vmState);
	void xret8v  (CVMState& vmState);
	void xret8m  (CVMState& vmState);
	void xret16v (CVMState& vmState);
	void xret16m (CVMState& vmState);
	void xret32v (CVMState& vmState);
	void xret32m (CVMState& vmState);

	// LEA
	void lea32mfm (CVMState& vmState);
	void lea32mfv (CVMState& vmState);
	void lea32mv (CVMState& vmState);

}