#pragma once
#include <map>
#include <vector>
#include "..\LIEng\typedefs.h"
using namespace interpreter;

////////////////////////////////////////////////////////////////////////////////
// “ип ссылки на значение (переменна€) или код (метка)
struct CVariableLink
{
	dword size;
	dword offset;
};

////////////////////////////////////////////////////////////////////////////////
// “ип вектора ссылок
typedef std::vector< CVariableLink > vct_dw;

// “ип функции-схемы трансл€ции
class  CTranslator;
struct CLexLine;
typedef void (CTranslator::*FTrsScheme)(CLexLine&);

// “ип вектора схем трансл€ции
typedef std::vector< FTrsScheme > vct_ftrs;

////////////////////////////////////////////////////////////////////////////////
