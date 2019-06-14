#include "Translator.h"
#include "LexThread.h"
#include "Reference.h"
#include "ReportAnError.h"

#include <iostream>
using namespace std;

//------------------------------------------------------------------------------
CTranslator::CTranslator(void)
{
}

//------------------------------------------------------------------------------
CTranslator::~CTranslator(void)
{
	for (refmap::iterator pos = mapReferences.begin(), end = mapReferences.end(); pos != end; pos++)
	{
		delete (*pos).second;
	}

	for (int i = 0, size = vctCommands.size(); i < size; i++)
	{
		delete vctCommands[i];
	}
}

//------------------------------------------------------------------------------
void CTranslator::Translate( CLexLine& line )
{
	if (!line.vct.size())
		return;

	// ���� ��� ���������, �� ���������� ���������� ������
	if ( DirectiveProcessing( line ) )
	{
		return;
	}

	CCommand* pCmd = new CCommand();
	pCmd->pRefVar = 0;

	if ( trsSchemeTable.CreateCommand( line, *pCmd, mapReferences ) )
	{
		// ������� �������
		vctCommands.push_back( pCmd );
		return;
	}
	else
	{
		// ������ ����� ����...

		const int size = line.vct.size();

		//------------------------------------------------------------------------------
		// �����/����������?
		try
		{
			delete pCmd;
			pCmd = 0;

			CReference *ref = new CReference( line, &pCmd );

			// ��� ���������� ��� ���� ���������?
			refmap::iterator pos = mapReferences.find( ref->strName );
			if ( pos != mapReferences.end() && (*pos).second->bImplemented )
			{
				ReportAnError( "Reference redefinition", line.nLineNumber );
			}

			// ��� ���������� ��� ���� ������������ �� �����������?
			if ( pos != mapReferences.end() )
			{
				mapReferences[ ref->strName ]->bImplemented = 1;
				mapReferences[ ref->strName ]->nLine        = line.nLineNumber;
				mapReferences[ ref->strName ]->pPlaceCmd    = ref->pPlaceCmd;
				mapReferences[ ref->strName ]->pType        = ref->pType;
				ref->pPlaceCmd->pRefVar = mapReferences[ ref->strName ];

				// ����������� �����
				ref->pType = 0;
				delete ref;
			}
			else
			{
				// ��������� ����� ����������
				mapReferences[ ref->strName ] = ref;
			}

			// ���������� ���������� �������� � ��������������
			// ����� ��� ���.
			if ( pCmd )
			{
				vctCommands.push_back( pCmd );
			}

		}
		// ��� �� ���������� � �� �����
		catch ( int )
		{
			ReportAnError( "Invalid command or directive found", line.nLineNumber );
		}
	}
}

//------------------------------------------------------------------------------
void CTranslator::Generate()
{
	// ��� �� ���������� ���������� � ������������?
	for ( refmap::iterator pos = mapReferences.begin(); pos != mapReferences.end(); pos++ )
	{
		// ����������, �� �� ������������
		if ( (*pos).second->bImplemented && !(*pos).second->bUsed )
		{
			cout
				<< "Warning: reference \""
				<< (*pos).second->strName
				<< "\" is defined but is not used at line: "
				<< (*pos).second->nLine
				<< endl;
		}

		// ������������, �� �� ����������
		if ( !(*pos).second->bImplemented && (*pos).second->bUsed )
		{
			ReportAnError( string("Undefined variable found: ") + (*pos).second->strName, (*pos).second->nLine );
		}
	}

	// �������� ������ � �������� ������ �������. ������ ��� ��� ����� �������.
	// ���������� ��� ���������� �������� ����������, ������������ ��� ��������� ����.
	unsigned long int dwCurrentOffset = 0;
	for (int i = 0, size = vctCommands.size(); i < size; i++)
	{
		vctCommands[i]->CalcOffset( dwCurrentOffset );
	}

	// ����������� ��������
	DirectiveRequestProcessing();

	// ���������!
	for (int i = 0, size = vctCommands.size(); i < size; i++)
	{
		vctCommands[i]->Generate( progInfo );
	}
}

//------------------------------------------------------------------------------
bool CTranslator::DirectiveProcessing( CLexLine& line )
{
	static const char strNameIsAlreadyUsed[] = "This name is already used at the another directive";

	const size_t size = line.vct.size();

	// ��������� ��������

	//------------------------------------------------------------------------------
	// import funcName: "module_name", "ext_func_name"
	if (
		line.vct[0].strSource == "import"
	&&	size == 6
	&&	line.vct[1].lexType == ltKeyWord
	&&	IsNameCorrect( line.vct[1].strSource )
	&&	line.vct[2].strSource == ":"
	&&	line.vct[3].lexType == ltString
	&&	line.vct[4].strSource == ","
	&&	line.vct[5].lexType == ltString
	)
	{
		// ��������� (�����) � ���� ���������� ��� �����������?
		if ( NameIsAlreadyDefined( line.vct[1].strSource ) )
		{
			ReportAnError( strNameIsAlreadyUsed, line.nLineNumber );
		}

		// ������������ ����� ����������������
		CImportProc ip;
		ip.id                = progInfo.vctImports.size();
		ip.strDefinitionName = line.vct[1].strSource;
		ip.strModule         = line.vct[3].strSource;
		ip.strName           = line.vct[5].strSource;

		progInfo.vctImports.push_back( ip );

		return 1;
	}

	//------------------------------------------------------------------------------
	// export externalLabelName: labelName, stack_size
	if ( line.vct[0].strSource == "export"
	&&	size == 6
	&&	line.vct[1].lexType == ltKeyWord
	&&	IsNameCorrect( line.vct[1].strSource )
	&&	line.vct[2].strSource == ":"
	&&	line.vct[3].lexType == ltKeyWord
	&&	line.vct[4].strSource == ","
	&&	line.vct[5].lexType == ltNumber
	)
	{
		// ��������� (�����) � ���� ���������� ��� �����������?
		if ( NameIsAlreadyDefined( line.vct[1].strSource ) )
		{
			ReportAnError( strNameIsAlreadyUsed, line.nLineNumber );
		}

		// ������������ ����� ����������������
		CExportProc ep;
		ep.dwId              = progInfo.vctExports.size();
		ep.strDefinitionName = line.vct[1].strSource;
		ep.strLabelName      = line.vct[3].strSource;
		ep.dwStackSize       = line.vct[5].nNumber;
		ep.dwOffset          = line.nLineNumber; // ��������!

		progInfo.vctExports.push_back( ep );

		return 1;
	}

	//------------------------------------------------------------------------------
	const size_t import_size = progInfo.vctImports.size();
	const size_t export_size = progInfo.vctExports.size();

	CLexeme lxDWORD;
	lxDWORD.strSource = "dword";
	lxDWORD.lexType   = ltKeyWord;

	CLexeme lxNumber;
	lxNumber.lexType   = ltNumber;

	// ������� �������� ����������� ��� ��������������, ��� ����� �����
	for (size_t i = 0; i < line.vct.size(); i++)
	{
		// import
		for (size_t k = 0; k < import_size; k++)
		{
			if ( progInfo.vctImports[k].strDefinitionName == line.vct[i].strSource )
			{
				lxNumber.nNumber = k;
				line.vct[i] = lxNumber;
				line.vct.insert( line.vct.begin() + i, lxDWORD );
			}
		}

		// export
		for (size_t k = 0; k < export_size; k++)
		{
			if ( progInfo.vctExports[k].strDefinitionName == line.vct[i].strSource )
			{
				lxNumber.nNumber = k;
				line.vct[i] = lxNumber;
				line.vct.insert( line.vct.begin() + i, lxDWORD );
 			}
		}
	}

	return 0;
}

//------------------------------------------------------------------------------
bool CTranslator::IsNameCorrect( const string& strName )
{
	// ���� ��� �� ��� ������� � �� ��� ���������, �� ����� ������������
	std::map< std::string, CTranslationScheme* >::iterator pos = trsSchemeTable.mapSchemes.find( strName );
	if ( pos == trsSchemeTable.mapSchemes.end() && strName != "import" && strName != "export" && strName != "dword" )
	{
		return 1;
	}

	return 0;
}

//------------------------------------------------------------------------------
bool CTranslator::NameIsAlreadyDefined( const std::string& strName )
{
	const size_t import_size = progInfo.vctImports.size();
	const size_t export_size = progInfo.vctExports.size();

	for (size_t i = 0; i < import_size; i++)
	{
		if ( progInfo.vctImports[i].strDefinitionName == strName )
			return 1;
	}

	for (size_t i = 0; i < export_size; i++)
	{
		if ( progInfo.vctExports[i].strDefinitionName == strName )
			return 1;
	}

	return 0;
}

//------------------------------------------------------------------------------
void CTranslator::DirectiveRequestProcessing()
{
	const size_t export_size = progInfo.vctExports.size();

	// ��� ������ ��������� �������� ������ �����, � ������� ��� ���������
	for (size_t i = 0; i < export_size; i++)
	{
		refmap::iterator pos = mapReferences.find( progInfo.vctExports[i].strLabelName );

		// ����� �� ���� ���������
		if ( pos == mapReferences.end() )
		{
			ReportAnError( "The label used in the directive was not declarated", progInfo.vctExports[i].dwOffset );
		}

		progInfo.vctExports[i].dwOffset = (*pos).second->pPlaceCmd->dwMyOffset;
	}
}

//------------------------------------------------------------------------------
