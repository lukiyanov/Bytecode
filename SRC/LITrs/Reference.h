#pragma once
#include "RefType.h"
#include <string>

class CCommand;

class CReference
{
public:
	int nLine;
	std::string strName;
	bool bImplemented;		// Переменная определена?
	bool bUsed;				// Переменная используется?

	CRefType* pType;		// Заполняется при нахождении реализации // <-- +Initial value
	CCommand* pPlaceCmd;	// Команда, генерирующая нашу переменную

public:
	CReference(void);
	// Использование переменной/метки
	CReference( std::string name );
	// Объявление переменной/метки
	CReference( const CLexLine& line, CCommand** pCmd );

	~CReference(void);
};
