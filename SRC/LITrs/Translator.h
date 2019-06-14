#pragma once
#include <map>
#include <vector>
#include "ProgramInfo.h"
#include "LexThread.h"
#include "TranslationSchemeTable.h"
#include "Command.h"
#include "Reference.h"

struct CLexLine;

////////////////////////////////////////////////////////////////////////////////
class CTranslator
{
	typedef std::map< std::string, CReference* > refmap;

	CProgramInfo progInfo;

	// ���������� (��� �����������, ��� � �������������):
	refmap mapReferences;

	// ����� ���������� ������:
	CTranslationSchemeTable trsSchemeTable;

	// ������� � ������� �� ����������
	std::vector< CCommand* > vctCommands;

public:
	CTranslator(void);
	~CTranslator(void);

	void Translate( CLexLine& line );

	void Generate();

	inline CProgramInfo& GetProgramInfo()
		{ return progInfo; }

private:
	// ��������� � �� ���������
	bool DirectiveProcessing( CLexLine& line );
	bool IsNameCorrect( const std::string& strName );
	bool NameIsAlreadyDefined( const std::string& strName );
	void DirectiveRequestProcessing();
};

////////////////////////////////////////////////////////////////////////////////
