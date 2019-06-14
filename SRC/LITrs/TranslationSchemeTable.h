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
// ѕараметры одного аргумента.
struct CArgumentParameters
{
	int  bits;
	RTTI type;
};

////////////////////////////////////////////////////////////////////////////////
struct CTranslationScheme
{
	ECOP COP;
	// ”казатель на начало массива параметров аргументов
	CArgumentParameters* pCombination;
};

////////////////////////////////////////////////////////////////////////////////
//  онтейнер дл€ схем транслиции команд (только команд!),
// использует статическую инициализацию.
struct CTranslationSchemeTable
{
	typedef std::map< std::string, CTranslationScheme* > schmap;

	// —писок транслируемых команд
	schmap mapSchemes;

public:
	CTranslationSchemeTable();

	// ¬озвращает 0, если команда не найдена
	bool CreateCommand(const CLexLine& line, /*OUT: */ CCommand& cmd, std::map< std::string, CReference* >& mapRef );
};

////////////////////////////////////////////////////////////////////////////////

