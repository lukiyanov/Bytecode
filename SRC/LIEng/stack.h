#pragma once
#include "VMState.h"

namespace interpreter {

	// Stack commands
	//------------------------------------------------------------------------------

	// � ����� ������ ��������� �������������� ����������, �.�. ������ � ���,
	// ����� ��� ����� pop, ����������. ���� ������������ ��� �������� ����������
	// � ���������.

	// ��������!
	// ��� ������� ������ �� ������ ����������� ����� ��������, �������� - ����� std::vector,
	// �� ������ �������� ���� � ����������. ��� ������ ������� �������� ����������� ������
	// ��� ��������� ��������������. (MAYBE TODO)

	// PUSH
	void push8v  (CVMState& vmState);
	void push16v (CVMState& vmState);
	void push32v (CVMState& vmState);

	void push8m  (CVMState& vmState);
	void push16m (CVMState& vmState);
	void push32m (CVMState& vmState);

	// POP
	void pop8m  (CVMState& vmState);
	void pop16m (CVMState& vmState);
	void pop32m (CVMState& vmState);

	// SPOP
	// (simple pop)
	// ������������ �������� �� ����� ��� ������ �� ����-����
	void spop     (CVMState& vmState);
	void spop16v  (CVMState& vmState);
	void spop16m  (CVMState& vmState);

	// LOCK
 	void lock  (CVMState& vmState);
 	void lockm (CVMState& vmState);

	// CALL
	void call32v (CVMState& vmState);
	void call32m (CVMState& vmState);

	// RET
	void ret    (CVMState& vmState);
	void ret8v  (CVMState& vmState);
	void ret8m  (CVMState& vmState);
	void ret16v (CVMState& vmState);
	void ret16m (CVMState& vmState);
	void ret32v (CVMState& vmState);
	void ret32m (CVMState& vmState);

	//------------------------------------------------------------------------------
}