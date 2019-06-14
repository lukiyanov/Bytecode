#include "Argument.h"
#include "Reference.h"
#include "Command.h"
#include "ReportAnError.h"

////////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC( CVariableArg )
IMPLEMENT_DYNAMIC( CValueArg )
IMPLEMENT_DYNAMIC( CPointerArg )

////////////////////////////////////////////////////////////////////////////////
CArgument* ConstructArg ( const CLexLine& args, std::map< std::string, CReference* >& mapRef )
{
	// ������ �� �����
	if ( !args.vct.size() )
	{
		ReportAnError( "Clear argument found", args.nLineNumber );
	}

	if ( CArgument* pResult = CVariableArg::Construct( args, mapRef ) )
	{
		return pResult;
	}

	if ( CArgument* pResult = CValueArg::Construct( args, mapRef ) )
	{
		return pResult;
	}

	if ( CArgument* pResult = CPointerArg::Construct( args, mapRef ) )
	{
		return pResult;
	}

	return 0;
}

////////////////////////////////////////////////////////////////////////////////
int GetTypeBit ( const std::string& str )
{
	if ( str == "byte" )
		return 8;
	if ( str == "word" )
		return 16;
	if ( str == "dword" )
		return 32;
	return 0;
}

////////////////////////////////////////////////////////////////////////////////
//								CONSTRUCT
////////////////////////////////////////////////////////////////////////////////
CVariableArg* CVariableArg::SimpleConstruct( const CLexLine& line, const int offset, const int backward, std::map< std::string, CReference* >& mapRef )
{
	typedef std::map< std::string, CReference* > refmap;

	const int size = line.vct.size();

	// ���� � ��� � ������� ���� ������� � ��� - ����.�����/ID
	if ( size == offset + 1 + backward && line.vct[offset].lexType == ltKeyWord )
	{
		CVariableArg *pArg = new CVariableArg();

		// ����������� ������� �� ������������
		pArg->bits = 0;

		// �������� ��������� �������� �� ������������
		pArg->dwAddress = 0;

		// ��������� ���������� � ����� ������

		refmap::iterator pos = mapRef.find( line.vct[offset].strSource );

		// ���������� � ����� ������ ��� ��������?
		if ( pos != mapRef.end() )
		{
			// ������, ��� ��� ������������
			(*pos).second->bUsed = 1;
			// � ��������� ��������� �� ���
			pArg->pVariableUsed = (*pos).second;
		}
		else
		{
			// ����� ���������� ����� �� �����������.

			// �������� ����������
			pArg->pVariableUsed = new CReference( line.vct[offset].strSource );
			// ������, ��� ��� ������������
			pArg->pVariableUsed->bUsed   = 1;
			// ������, ��� ��� ������������
			pArg->pVariableUsed->nLine   = line.nLineNumber;
			// ������� �� � ����� ������
			mapRef[ line.vct[offset].strSource ] = pArg->pVariableUsed;
		}

		return pArg;
	}

	return 0;
}

CVariableArg* CVariableArg::Construct( const CLexLine& line, std::map< std::string, CReference* >& mapRef )
{
	// ������� ��������:
	// 1. var_name
	// 2. type_name var_name
	// 3. type_name + '[' + number + ']'

	const int size = line.vct.size();

	int bits = GetTypeBit( line.vct[0].strSource );


	if ( bits )
	{
		// ARGTYPE:
		// 1. type_name var_name
		// 2. type_name + '[' + number + ']'

		CVariableArg* pArg = SimpleConstruct( line, 1, 0, mapRef );

		// ARGTYPE:
		// 1. type_name var_name?
		if ( pArg )
		{
			// ����������� ������� �������� ��������
			pArg->bits = bits;
			return pArg;
		}

		// ARGTYPE:
		// 1. type_name + '[' + number + ']' ?
		if (
			size != 4 ||
			line.vct[1].strSource != "[" ||
			line.vct[2].lexType   != ltNumber ||
			line.vct[3].strSource != "]" )
			return 0;

		// ������ ������ � ������ ������ ��������� �� ���������
		pArg = new CVariableArg();
		pArg->dwAddress = line.vct[2].nNumber;
		pArg->bits = bits;
		return pArg;
	}
	else
	{
		// ARGTYPE:
		// 1. var_name
		CVariableArg* pArg = SimpleConstruct( line, 0, 0, mapRef );
		return pArg;
	}

	return 0;
}

////////////////////////////////////////////////////////////////////////////////
CValueArg* CValueArg::Construct( const CLexLine& line, std::map< std::string, CReference* >& mapRef )
{
	// 1. type_name number

	const int size = line.vct.size();

	int bits = GetTypeBit( line.vct[0].strSource );

	if ( !bits || size != 2 || line.vct[1].lexType != ltNumber )
	{
		return 0;
	}

	CValueArg* pArg = new CValueArg();
	pArg->bits = bits;
	pArg->nValue = line.vct[1].nNumber;
	return pArg;
}

////////////////////////////////////////////////////////////////////////////////
CPointerArg* CPointerArg::Construct( const CLexLine& line, std::map< std::string, CReference* >& mapRef )
{
// 1. type [var]
// 2. type [var+n]
// 3. type [var-n]

	typedef std::map< std::string, CReference* > refmap;
	const int size = line.vct.size();

	int bits = GetTypeBit( line.vct[0].strSource );

	if ( !bits || size < 4 || line.vct[1].strSource != "[" || line.vct[2].lexType != ltKeyWord )
	{
		return 0;
	}

	int offset = 0;
	if ( line.vct[3].strSource == "+" )
	{
		// + offset ]
		if ( size != 6 || line.vct[4].lexType != ltNumber || line.vct[5].strSource != "]" )
			return 0;

		offset = line.vct[4].nNumber;
	}
	else if ( line.vct[3].strSource == "-" )
	{
		// - offset ]
		if ( size != 6 || line.vct[4].lexType != ltNumber || line.vct[5].strSource != "]" )
			return 0;

		offset = - (signed short int) line.vct[4].nNumber;
	}
	else
	{
		// ]
		if ( line.vct[3].strSource != "]" )
			return 0;

		offset = 0;
	}

	CPointerArg *pArg = new CPointerArg();

	// ����������� ������� ������������ �����������
	pArg->bits = bits;

	// ��������
	pArg->offset = offset;

	// ����� ������
	pArg->nLine = line.nLineNumber;


	// ��������� ���������� � ����� ������

	refmap::iterator pos = mapRef.find( line.vct[2].strSource );

	// ���������� � ����� ������ ��� ��������?
	if ( pos != mapRef.end() )
	{
		// ������, ��� ��� ������������
		(*pos).second->bUsed = 1;
		// � ��������� ��������� �� ���
		pArg->pVariableUsed = (*pos).second;
	}
	else
	{
		// ����� ���������� ����� �� �����������.

		// �������� ����������
		pArg->pVariableUsed = new CReference( line.vct[2].strSource );
		// ������, ��� ��� ������������
		pArg->pVariableUsed->bUsed   = 1;
		// ������, ��� ��� ������������
		pArg->pVariableUsed->nLine   = line.nLineNumber;
		// ������� �� � ����� ������
		mapRef[ line.vct[2].strSource ] = pArg->pVariableUsed;
	}

	return pArg;
}

////////////////////////////////////////////////////////////////////////////////
//								GENERATE
////////////////////////////////////////////////////////////////////////////////
void CVariableArg::Generate( CProgramInfo& pi )
{
	unsigned long varAddr;

	// �������� ������� ��������?
	if ( dwAddress )
	{
		// �� - ���������� ��������
		varAddr = dwAddress;
	}
	else
	{
		// ��� - ������� ��� ����������
		varAddr = pVariableUsed->pPlaceCmd->dwMyOffset;
	}

	char *p = (char*)(&varAddr);
	pi.vctCode.push_back( p[0] );
	pi.vctCode.push_back( p[1] );
	pi.vctCode.push_back( p[2] );
	pi.vctCode.push_back( p[3] );
}

////////////////////////////////////////////////////////////////////////////////
void CValueArg::Generate( CProgramInfo& pi )
{
	char *p = (char*)(&nValue);

	int byte_count = bits / 8;
	for (int i = 0; i < byte_count; i++)
	{
		pi.vctCode.push_back( p[i] );
	}
}

////////////////////////////////////////////////////////////////////////////////
void CPointerArg::Generate( CProgramInfo& pi )
{
	unsigned long addr = pVariableUsed->pPlaceCmd->dwMyOffset;
	char *p = (char*)(&addr);
	pi.vctCode.push_back( p[0] );
	pi.vctCode.push_back( p[1] );
	pi.vctCode.push_back( p[2] );
	pi.vctCode.push_back( p[3] );

	p = (char*)(&offset);
	pi.vctCode.push_back( p[0] );
	pi.vctCode.push_back( p[1] );
}

////////////////////////////////////////////////////////////////////////////////
//							GET [TARGET] BITS
////////////////////////////////////////////////////////////////////////////////
unsigned long CVariableArg::GetTargetBits()
{
	if ( bits )
		return bits;
	else
		return pVariableUsed->pType->nBits;
}

////////////////////////////////////////////////////////////////////////////////
unsigned long CVariableArg::GetBits()
{
	return 32; // ������ ������ ����������
}

////////////////////////////////////////////////////////////////////////////////
unsigned long CValueArg::GetTargetBits()
{
	return bits;
}

////////////////////////////////////////////////////////////////////////////////
unsigned long CValueArg::GetBits()
{
	return bits;
}

////////////////////////////////////////////////////////////////////////////////
unsigned long CPointerArg::GetTargetBits()
{
	return bits;
}

////////////////////////////////////////////////////////////////////////////////
unsigned long CPointerArg::GetBits()
{
	if ( pVariableUsed->pType->nBits != 32 )
		ReportAnError( "Pointer argument must be a dword variable", nLine );
	return 6 * 8; // ( sizeof(COP) + sizeof(offset) ) * bit_count( byte )
}

////////////////////////////////////////////////////////////////////////////////

