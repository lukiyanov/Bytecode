#pragma once
#include "VMState.h"

namespace interpreter {
	// Сравнения и условные переходы
	//------------------------------------------------------------------------------

	// Команды cmpxx устанавливают флаг bct в true или false,
	// Синтаксис:
	// cmpxx <op1>, <op2>
	// op1, op2 - 8/16/32, m/v, но не оба v (~vv бессмысленно)
	//
	// jct <addr> - переход на метку addr, если (bct == true)
	// jcf <addr> - переход на метку addr, если (bct == false)
	// addr - 32, m/v

	// CMPE
	void cmpe8mm	(CVMState& vmState);
	void cmpe8mv	(CVMState& vmState);
	void cmpe8vm	(CVMState& vmState);

	void cmpe16mm	(CVMState& vmState);
	void cmpe16mv	(CVMState& vmState);
	void cmpe16vm	(CVMState& vmState);

	void cmpe32mm	(CVMState& vmState);
	void cmpe32mv	(CVMState& vmState);
	void cmpe32vm	(CVMState& vmState);

	// CMPNE
	void cmpne8mm	(CVMState& vmState);
	void cmpne8mv	(CVMState& vmState);
	void cmpne8vm	(CVMState& vmState);

	void cmpne16mm	(CVMState& vmState);
	void cmpne16mv	(CVMState& vmState);
	void cmpne16vm	(CVMState& vmState);

	void cmpne32mm	(CVMState& vmState);
	void cmpne32mv	(CVMState& vmState);
	void cmpne32vm	(CVMState& vmState);

	// CMPB
	void cmpb8mm	(CVMState& vmState);
	void cmpb8mv	(CVMState& vmState);
	void cmpb8vm	(CVMState& vmState);

	void cmpb16mm	(CVMState& vmState);
	void cmpb16mv	(CVMState& vmState);
	void cmpb16vm	(CVMState& vmState);

	void cmpb32mm	(CVMState& vmState);
	void cmpb32mv	(CVMState& vmState);
	void cmpb32vm	(CVMState& vmState);

	// CMPBE
	void cmpbe8mm	(CVMState& vmState);
	void cmpbe8mv	(CVMState& vmState);
	void cmpbe8vm	(CVMState& vmState);

	void cmpbe16mm (CVMState& vmState);
	void cmpbe16mv (CVMState& vmState);
	void cmpbe16vm (CVMState& vmState);

	void cmpbe32mm (CVMState& vmState);
	void cmpbe32mv (CVMState& vmState);
	void cmpbe32vm (CVMState& vmState);

	// CMPL
	void cmpl8mm	(CVMState& vmState);
	void cmpl8mv	(CVMState& vmState);
	void cmpl8vm	(CVMState& vmState);

	void cmpl16mm	(CVMState& vmState);
	void cmpl16mv	(CVMState& vmState);
	void cmpl16vm	(CVMState& vmState);

	void cmpl32mm	(CVMState& vmState);
	void cmpl32mv	(CVMState& vmState);
	void cmpl32vm	(CVMState& vmState);

	// CMPLE
	void cmple8mm	(CVMState& vmState);
	void cmple8mv	(CVMState& vmState);
	void cmple8vm	(CVMState& vmState);

	void cmple16mm (CVMState& vmState);
	void cmple16mv (CVMState& vmState);
	void cmple16vm (CVMState& vmState);

	void cmple32mm (CVMState& vmState);
	void cmple32mv (CVMState& vmState);
	void cmple32vm (CVMState& vmState);

	// JCx
	void jct32v (CVMState& vmState);
	void jct32m (CVMState& vmState);
	void jcf32v (CVMState& vmState);
	void jcf32m (CVMState& vmState);

	//------------------------------------------------------------------------------
}