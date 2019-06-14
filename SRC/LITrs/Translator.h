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

	// ѕеременные (как объ€вленные, так и запрашиваемые):
	refmap mapReferences;

	// —хемы трансл€ции команд:
	CTranslationSchemeTable trsSchemeTable;

	//  оманды в очереди на трансл€цию
	std::vector< CCommand* > vctCommands;

public:
	CTranslator(void);
	~CTranslator(void);

	void Translate( CLexLine& line );

	void Generate();

	inline CProgramInfo& GetProgramInfo()
		{ return progInfo; }

private:
	// ƒирективы и их обработка
	bool DirectiveProcessing( CLexLine& line );
	bool IsNameCorrect( const std::string& strName );
	bool NameIsAlreadyDefined( const std::string& strName );
	void DirectiveRequestProcessing();
};

////////////////////////////////////////////////////////////////////////////////
