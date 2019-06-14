#pragma once
#include <vector>
#include "ProgramInfo.h"

struct CTranslationScheme;
class CArgument;
class CReference;

class CCommand
{
public:
	int nLine;
	std::vector< CArgument* > vctArguments;
	CTranslationScheme* pTSArray;

	// ���� ������� ������ ������������ ����������,
	// �������� ��������� �� ���, ����� 0
	CReference* pRefVar;

	// �������� �������� ������� �� ������ ����-����
	// ����������� ��������������� ����� ���������� ����
	unsigned long int dwMyOffset;

public:
	void Generate( CProgramInfo& pi );
	void CalcOffset( unsigned long int& dwCurrentOffset );
};