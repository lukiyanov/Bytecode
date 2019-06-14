#include "RefType.h"
#include "ReportAnError.h"
#include "ProgramInfo.h"

//------------------------------------------------------------------------------
CRefType::CRefType(void){}
CRefType::~CRefType(void){}
CByte::CByte(void){}
CWord::CWord(void){}
CDWord::CDWord(void){}
CLabel::CLabel(){}

//------------------------------------------------------------------------------
const char szInvData[] = "Invalid data definition";
const char szInvValue[] = "Invalid data value definition";

//------------------------------------------------------------------------------
CByte* CByte::Construct( const CLexLine& line, const int offset )
{
	const int size = line.vct.size();
	const int truesize = offset + 2;

	if ( size < offset + 1 )
	{
		return 0;
	}

	if ( line.vct[ offset ].strSource == "db" )
	{

		if ( size != truesize )
		{
			ReportAnError( szInvData, line.nLineNumber );
		}

		if ( line.vct[ offset + 1 ].lexType == ltNumber )
		{
			CByte* p = new CByte();
			p->byInitial = (unsigned char)line.vct[ offset + 1 ].nNumber;
			p->nBits = 8;
			p->nAddr = 0;
			return p;
		}
		// Строка
		else if ( line.vct[ offset + 1 ].lexType == ltString && line.vct[ offset + 1 ].strSource.size() )
		{
			CByte* p = new CByte();
			p->strArray = line.vct[ offset + 1 ].strSource;
			p->byInitial = 0;
			p->nBits = 8;
			p->nAddr = 0;
			return p;
		}

		ReportAnError( szInvValue, line.nLineNumber );
	}

	return 0;
}

//------------------------------------------------------------------------------
CWord* CWord::Construct( const CLexLine& line, const int offset )
{
	const int size = line.vct.size();
	const int truesize = offset + 2;

	if ( size < offset + 1 )
	{
		return 0;
	}

	if ( line.vct[ offset ].strSource == "dw" )
	{

		if ( size != truesize )
		{
			ReportAnError( szInvData, line.nLineNumber );
		}

		if ( line.vct[ offset + 1 ].lexType != ltNumber )
		{
			ReportAnError( szInvValue, line.nLineNumber );
		}

		CWord* p = new CWord();
		p->wInitial = (unsigned short)line.vct[ offset + 1 ].nNumber;
		p->nBits = 16;
		p->nAddr = 0;
		return p;
	}

	return 0;
}

//------------------------------------------------------------------------------
CDWord* CDWord::Construct( const CLexLine& line, const int offset )
{
	const int size = line.vct.size();
	const int truesize = offset + 2;

	if ( size < offset + 1 )
	{
		return 0;
	}

	if ( line.vct[ offset ].strSource == "dd" )
	{

		if ( size != truesize )
		{
			ReportAnError( szInvData, line.nLineNumber );
		}

		if ( line.vct[ offset + 1 ].lexType != ltNumber )
		{
			ReportAnError( szInvValue, line.nLineNumber );
		}

		CDWord* p = new CDWord();
		p->dwInitial = line.vct[ offset + 1 ].nNumber;
		p->nBits = 32;
		p->nAddr = 0;
		return p;
	}

	return 0;
}

//------------------------------------------------------------------------------
CRefType* ConstructType( const CLexLine& line, const int offset )
{
	if ( CRefType* pResult = CByte::Construct( line, offset ) )
	{
		return pResult;
	}

	if ( CRefType* pResult = CWord::Construct( line, offset ) )
	{
		return pResult;
	}

	if ( CRefType* pResult = CDWord::Construct( line, offset ) )
	{
		return pResult;
	}

	return 0;
}

//------------------------------------------------------------------------------
void CByte::Generate( CProgramInfo& pi )
{
	const size_t size = strArray.size();
	if ( size )
	{
		for (size_t i = 0; i < size; i++)
			pi.vctCode.push_back( strArray[i] );
	}
	else
		pi.vctCode.push_back( byInitial );
}

//------------------------------------------------------------------------------
void CWord::Generate( CProgramInfo& pi )
{
	char *p = (char*)(&wInitial);
	pi.vctCode.push_back( p[0] );
	pi.vctCode.push_back( p[1] );
}

//------------------------------------------------------------------------------
void CDWord::Generate( CProgramInfo& pi )
{
	char *p = (char*)(&dwInitial);
	pi.vctCode.push_back( p[0] );
	pi.vctCode.push_back( p[1] );
	pi.vctCode.push_back( p[2] );
	pi.vctCode.push_back( p[3] );
}

////////////////////////////////////////////////////////////////////////////////
CLabel* CLabel::Construct( const CLexLine& line, const int )
{
	const int size = line.vct.size();

//	if ( line.vct[0].strSource == "label" )
//	{
		if ( size == 1 )
		{
			ReportAnError( "Label name is not specified", line.nLineNumber );
		}

		if ( line.vct[1].lexType != ltKeyWord )
		{
			ReportAnError( "Invalid label name is specified", line.nLineNumber );
		}

		if ( size == 2 || line.vct[2].strSource != ":" )
		{
			ReportAnError( "You forget an \':\' symbol after the label name", line.nLineNumber );
		}

		if ( size > 3 )
		{
			ReportAnError( "Inadmissible symbols found after the label definition", line.nLineNumber );
		}

		CLabel *pType = new CLabel();
		pType->nBits = 32;
		return pType;
//	}

	return 0;
}

////////////////////////////////////////////////////////////////////////////////
void CLabel::Generate( CProgramInfo& pi )
{
	; // Нечего генерировать
}

////////////////////////////////////////////////////////////////////////////////
unsigned long CLabel::GetTranslatedSize()
{
	// Размера не занимаем
	return 0;
}

////////////////////////////////////////////////////////////////////////////////
unsigned long CByte::GetTranslatedSize()
{
	const size_t size = strArray.size();
	if ( size )
		return size;
	return 1;
}