#include "LexThread.h"
#include <ctype.h> 
using namespace std;

//------------------------------------------------------------------------------
unsigned long ConvertToNumber( const std::string& str )
{
	if ( str[ str.length() - 1 ] == 'h' || str[ str.length() - 1 ] == 'H' )
	{
		// HEX
		char* p;
		return strtoul( str.c_str(), &p, 16 );
	}
	else
	{
		// DEC
		char* p;
		return strtoul( str.c_str(), &p, 10 );
	}
}

//------------------------------------------------------------------------------
CLexThread::CLexThread( std::ifstream& fsIn ):
	m_fsIn(fsIn)
{
	line.nLineNumber = 0;
}

//------------------------------------------------------------------------------
CLexThread::~CLexThread(void)
{
}

//------------------------------------------------------------------------------
CLexLine& CLexThread::GetLine()
{
	return line;
}

//------------------------------------------------------------------------------
void CLexThread::ParseNextLine()
{
	static const int bufsize = 1024; // ������������ ����� ������
	static char buf[ bufsize ];

	do
	{
		line.nLineNumber++;
		m_fsIn.getline( buf, bufsize, '\n' );
	}
	while ( m_fsIn && !line.ParseString( buf ) );

	bGoodBit = m_fsIn.good();
}

//------------------------------------------------------------------------------
void ReportNumber	 ( const string& str, int line );
void ReportCutString ( const string& str, int line );
void ReportChar		 ( const char ch,	  int line );

//------------------------------------------------------------------------------
// ���������� 0, ���� ������ ��������� �����
bool CLexLine::ParseString( const string& strLine )
{
	vct.clear();

	CLexeme lexeme;
	bool bIsString = 0;
	char chStr = 0;

	bool bHexCharFound = 0;
	bool bHexEndFound = 0;

	// --------------------------------
	for( int i = 0, size = strLine.size(); i < size; i++ )
	{
		// ������� ������
		const char ch = strLine[i];

		// --------------------------------
		// ��������� �����
		if ( bIsString )
		{
			if ( ch == chStr )
			{
				vct.push_back( lexeme );
				lexeme.strSource.clear();
				bIsString = 0;
				continue;
			}

			lexeme.strSource += ch;
			continue;
		}
		else
		{
			if ( ch == '\'' || ch == '\"' )
			{
				lexeme.lexType = ltString;
				chStr = ch;
				bIsString = 1;
				continue;
			}
		}

		// --------------------------------
		// ���� �� �����, �� ��� �� ������

		// --------------------------------
		// ���� ��������� ������ �����������, ...
		if ( ch == ';' )
		{
			// ...�� ��������� ����, �� ����������� ������� ������
			break;
		}

		// --------------------------------
		// ���� ��������� ���-�� �� �������� ��������, ������� �� ������
		if ( ((unsigned char)ch) > 127 )
		{
			ReportChar( ch, nLineNumber );
		}

		// --------------------------------
		// ���� ��������� ���������� ������, �� ������� ��������
		if ( isspace( ch ) )
		{
			if ( !lexeme.strSource.empty() )
			{
				if ( lexeme.lexType == ltNumber	&&  bHexCharFound && !bHexEndFound )
				{
					ReportNumber( lexeme.strSource, nLineNumber );
				}

				vct.push_back( lexeme );
				lexeme.strSource.clear();
			}

			continue;
		}

		// --------------------------------
		// ��������� �����������, ����� \' � \"
		if ( ispunct( ch ) && ch != '_' )
		{
			if ( !lexeme.strSource.empty() )
			{
				if ( lexeme.lexType == ltNumber	&&  bHexCharFound && !bHexEndFound )
				{
					ReportNumber( lexeme.strSource, nLineNumber );
				}

				vct.push_back( lexeme );
				lexeme.strSource.clear();
			}

			lexeme.lexType = ltDivider;
			lexeme.strSource = ch;
			vct.push_back( lexeme );
			lexeme.strSource.clear();

			continue;
		}

		// --------------------------------
		if ( lexeme.strSource.empty() )
		{
			// --------------------------------
			if ( isalpha( ch ) || ch == '_' )
			{
				// �������� �������� �������� ����� ��� �������������
				lexeme.lexType = ltKeyWord;
			}

			// --------------------------------
			else if ( isdigit( ch ) )
			{
				// �������� �������� �����
				lexeme.lexType = ltNumber;
				bHexCharFound = 0;
				bHexEndFound = 0;
			}

			// --------------------------------
			else
			{
				// ���������, ��� ��� �����
				ReportChar( ch, nLineNumber );
			}
		}

		// --------------------------------
		// �������� ������������ �����
		if ( lexeme.lexType == ltNumber )
		{
			// ����� x16 ������?
			if ( isxdigit( ch ) && !isdigit( ch ) )
			{
				bHexCharFound = 1;
			}

			else if ( ch == 'h' || ch == 'H' )
			{
				if ( bHexEndFound )
				{
					// 'h' �������� ������
					ReportNumber( lexeme.strSource + ch, nLineNumber );
				}

				bHexEndFound = 1;
			}
		}

		lexeme.strSource += ch;

	}	// for (...)

	// --------------------------------
	// ������� ��������� �������
	if ( !lexeme.strSource.empty() )
	{
		if ( lexeme.lexType == ltString )
		{
			ReportCutString( lexeme.strSource, nLineNumber );
		}

		if ( lexeme.lexType == ltNumber	&&  bHexCharFound && !bHexEndFound )
		{
			ReportNumber( lexeme.strSource, nLineNumber );
		}

		vct.push_back( lexeme );
	}

	// --------------------------------
	// ���������� ������ ltKeyWord � ������� ��������
	// ���������� ������ ltNumber � ������� ��������
	for (int i = 0, size = vct.size(); i < size; i++)
	{
		if ( vct[i].lexType == ltKeyWord )
		{
			for (int k = 0, length = vct[i].strSource.length(); k < length; k++)
			{
				vct[i].strSource[k] = tolower( vct[i].strSource[k] );
			}
		}
		else if ( vct[i].lexType == ltNumber )
		{
			vct[i].nNumber = ConvertToNumber( vct[i].strSource );
		}
	}

	// --------------------------------
	return !vct.empty();
}

//------------------------------------------------------------------------------
// ��� �������� ������ ������ � char[]
static const int bufsize = 128;
static char buf[bufsize];

//------------------------------------------------------------------------------
void ReportCutString( const string& cstr, int line )
{
	string str = "Cut string: \"";
	str += cstr;
	str += "\" at line: ";
	_itoa_s( line, buf, bufsize, 10 );
	str += buf;
	throw exception( str.c_str() );
}

//------------------------------------------------------------------------------
void ReportChar( const char ch, int line )
{
	string str = "Inadmissible character found: \'";
	str += ch;
	str += "\' at line: ";
	_itoa_s( line, buf, bufsize, 10 );
	str += buf;
	throw exception( str.c_str() );
}

//------------------------------------------------------------------------------
void ReportNumber( const string& nstr, int line )
{
	string str = "Inadmissible number writing found: \'";
	str += nstr;
	str += "\' at line: ";
	_itoa_s( line, buf, bufsize, 10 );
	str += buf;
	throw exception( str.c_str() );
}

//------------------------------------------------------------------------------
