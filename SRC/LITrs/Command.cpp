#include <stdlib.h>
#include <exception>
#include <windows.h>

#include "Command.h"
#include "Argument.h"
#include "RefType.h"
#include "Reference.h"
#include "ReportAnError.h"
#include "TranslationSchemeTable.h"

using std::exception;

const int bufsize = 64;
char buf[ bufsize ];

////////////////////////////////////////////////////////////////////////////////
void CCommand::CalcOffset( unsigned long int& dwCurrentOffset )
{
	dwMyOffset = dwCurrentOffset;

	if ( pRefVar )
	{
		// Размер переменной
		dwCurrentOffset = dwCurrentOffset + pRefVar->pType->GetTranslatedSize();
	}
	else
	{
		// Размер команды
		dwCurrentOffset += 2; // Размер команды

		const int size = vctArguments.size();

		// Теперь найдем схему трансляции, под которую подходит
		// наш список аргументов
		CTranslationScheme* pMyScheme = 0;
		for (CTranslationScheme* pScheme = pTSArray; pScheme->pCombination; pScheme++)
		{
			bool bUsable = 1;

			CArgumentParameters* pArgSet = pScheme->pCombination;

			// Находим количество агрументов данной схемы
			int nTrsArgCount = 0;
			for (; (pArgSet + nTrsArgCount)->bits; nTrsArgCount++);

			// Количество аргументов не совпало
			if ( size != nTrsArgCount )
				continue;

			// Тип каждого аргумента проверяем на совпадение
			for (int i = 0; i < size; i++)
			{
				// AHTUNG!!! CLabel должен восприниматься как непосредственное
				// значение, а не как адрес переменной
				RTTI pType           = vctArguments[i]->GetRuntimeClass();
				unsigned long dwBits = vctArguments[i]->GetTargetBits();
				{
					// REDO: Переделать эту жуть к черту!!!
					CVariableArg* pArg = dynamic_cast< CVariableArg* >( vctArguments[i] );
					if ( pArg && pArg->pVariableUsed && pArg->pVariableUsed->pType )
					{
						if ( !pArg->pVariableUsed->pType->GetTranslatedSize() )
						{
							pType = RUNTIME_CLASS( CValueArg );
						}
					}
				}

				// Если не совпал тип или размер операнда...
				if (
					 pArgSet[i].type != pType
				||   pArgSet[i].bits != dwBits
				)
				{
					// ... то эта схема нам не подходит
					bUsable = 0;
					break;
				}
			}

			// Нашли?
			if ( bUsable )
			{
				pMyScheme = pScheme;
				break;
			}
		}

		// Не нашли подходящей схемы
		if ( !pMyScheme )
		{
			ReportAnError( "Invalid command arguments", nLine );
		}

		// Сохраняем результат
		pTSArray = pMyScheme;

		// Прибавляем размер каждого аргумента
		for (int i = 0; i < size; i++)
		{
			dwCurrentOffset += vctArguments[i]->GetBits() / 8;
		}

	} // else
}

////////////////////////////////////////////////////////////////////////////////
void CCommand::Generate( CProgramInfo& pi )
{
	const int size = vctArguments.size();

	// Если это - переменная, генерируем соответствующий код
	if ( pRefVar )
	{
		pRefVar->pType->Generate( pi );
	}
	else
	{
		// Транслируем сначала саму команду...
		short int cop = pTSArray->COP;
		pi.vctCode.push_back( (char)LOBYTE( cop ) );
		pi.vctCode.push_back( (char)HIBYTE( cop ) );

		// ...а затем каждый из ее параметров
		for (int i = 0; i < size; i++)
		{
			vctArguments[i]->Generate( pi );
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
