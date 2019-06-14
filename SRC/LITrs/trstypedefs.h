#pragma once
#include <map>
#include <vector>
#include "..\LIEng\typedefs.h"
using namespace interpreter;

////////////////////////////////////////////////////////////////////////////////
// ��� ������ �� �������� (����������) ��� ��� (�����)
struct CVariableLink
{
	dword size;
	dword offset;
};

////////////////////////////////////////////////////////////////////////////////
// ��� ������� ������
typedef std::vector< CVariableLink > vct_dw;

// ��� �������-����� ����������
class  CTranslator;
struct CLexLine;
typedef void (CTranslator::*FTrsScheme)(CLexLine&);

// ��� ������� ���� ����������
typedef std::vector< FTrsScheme > vct_ftrs;

////////////////////////////////////////////////////////////////////////////////
