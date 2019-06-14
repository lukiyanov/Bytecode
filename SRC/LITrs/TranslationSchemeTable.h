#pragma once

#include "Command.h"
#include "..\LIEng\COP.h"
#include "LexThread.h"
#include "trstypedefs.h"
#include "Argument.h"

#include <string>
#include <map>

class CReference;

////////////////////////////////////////////////////////////////////////////////
// ��������� ������ ���������.
struct CArgumentParameters
{
	int  bits;
	RTTI type;
};

////////////////////////////////////////////////////////////////////////////////
struct CTranslationScheme
{
	ECOP COP;
	// ��������� �� ������ ������� ���������� ����������
	CArgumentParameters* pCombination;
};

////////////////////////////////////////////////////////////////////////////////
// ��������� ��� ���� ���������� ������ (������ ������!),
// ���������� ����������� �������������.
struct CTranslationSchemeTable
{
	typedef std::map< std::string, CTranslationScheme* > schmap;

	// ������ ������������� ������
	schmap mapSchemes;

public:
	CTranslationSchemeTable();

	// ���������� 0, ���� ������� �� �������
	bool CreateCommand(const CLexLine& line, /*OUT: */ CCommand& cmd, std::map< std::string, CReference* >& mapRef );
};

////////////////////////////////////////////////////////////////////////////////

