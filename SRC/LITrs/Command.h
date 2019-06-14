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

	// Если команда должна генерировать переменную,
	// содержит указатель на нее, иначе 0
	CReference* pRefVar;

	// Содержит смещение команды от начала байт-кода
	// Вычисляется непосредственно перед генерацией кода
	unsigned long int dwMyOffset;

public:
	void Generate( CProgramInfo& pi );
	void CalcOffset( unsigned long int& dwCurrentOffset );
};