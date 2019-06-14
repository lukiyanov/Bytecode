#pragma once

#include "VMState.h"

namespace interpreter {
	//------------------------------------------------------------------------------
	// MOVSX
	void movsx16x8mm  (CVMState& vmState);
	void movsx32x8mm  (CVMState& vmState);
	void movsx32x16mm (CVMState& vmState);
	// MOVSZ
	void movzx16x8mm  (CVMState& vmState);
	void movzx32x8mm  (CVMState& vmState);
	void movzx32x16mm (CVMState& vmState);

	//------------------------------------------------------------------------------
}