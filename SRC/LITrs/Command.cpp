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
		// ������ ����������
		dwCurrentOffset = dwCurrentOffset + pRefVar->pType->GetTranslatedSize();
	}
	else
	{
		// ������ �������
		dwCurrentOffset += 2; // ������ �������

		const int size = vctArguments.size();

		// ������ ������ ����� ����������, ��� ������� ��������
		// ��� ������ ����������
		CTranslationScheme* pMyScheme = 0;
		for (CTranslationScheme* pScheme = pTSArray; pScheme->pCombination; pScheme++)
		{
			bool bUsable = 1;

			CArgumentParameters* pArgSet = pScheme->pCombination;

			// ������� ���������� ���������� ������ �����
			int nTrsArgCount = 0;
			for (; (pArgSet + nTrsArgCount)->bits; nTrsArgCount++);

			// ���������� ���������� �� �������
			if ( size != nTrsArgCount )
				continue;

			// ��� ������� ��������� ��������� �� ����������
			for (int i = 0; i < size; i++)
			{
				// AHTUNG!!! CLabel ������ �������������� ��� ����������������
				// ��������, � �� ��� ����� ����������
				RTTI pType           = vctArguments[i]->GetRuntimeClass();
				unsigned long dwBits = vctArguments[i]->GetTargetBits();
				{
					// REDO: ���������� ��� ���� � �����!!!
					CVariableArg* pArg = dynamic_cast< CVariableArg* >( vctArguments[i] );
					if ( pArg && pArg->pVariableUsed && pArg->pVariableUsed->pType )
					{
						if ( !pArg->pVariableUsed->pType->GetTranslatedSize() )
						{
							pType = RUNTIME_CLASS( CValueArg );
						}
					}
				}

				// ���� �� ������ ��� ��� ������ ��������...
				if (
					 pArgSet[i].type != pType
				||   pArgSet[i].bits != dwBits
				)
				{
					// ... �� ��� ����� ��� �� ��������
					bUsable = 0;
					break;
				}
			}

			// �����?
			if ( bUsable )
			{
				pMyScheme = pScheme;
				break;
			}
		}

		// �� ����� ���������� �����
		if ( !pMyScheme )
		{
			ReportAnError( "Invalid command arguments", nLine );
		}

		// ��������� ���������
		pTSArray = pMyScheme;

		// ���������� ������ ������� ���������
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

	// ���� ��� - ����������, ���������� ��������������� ���
	if ( pRefVar )
	{
		pRefVar->pType->Generate( pi );
	}
	else
	{
		// ����������� ������� ���� �������...
		short int cop = pTSArray->COP;
		pi.vctCode.push_back( (char)LOBYTE( cop ) );
		pi.vctCode.push_back( (char)HIBYTE( cop ) );

		// ...� ����� ������ �� �� ����������
		for (int i = 0; i < size; i++)
		{
			vctArguments[i]->Generate( pi );
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
