#pragma once
#include "RefType.h"
#include <string>

class CCommand;

class CReference
{
public:
	int nLine;
	std::string strName;
	bool bImplemented;		// ���������� ����������?
	bool bUsed;				// ���������� ������������?

	CRefType* pType;		// ����������� ��� ���������� ���������� // <-- +Initial value
	CCommand* pPlaceCmd;	// �������, ������������ ���� ����������

public:
	CReference(void);
	// ������������� ����������/�����
	CReference( std::string name );
	// ���������� ����������/�����
	CReference( const CLexLine& line, CCommand** pCmd );

	~CReference(void);
};
