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
	static const int bufsize = 1024; // максимальная длина строки
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
// Возвращает 0, если строка оказалась пуста
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
		// Текущий символ
		const char ch = strLine[i];

		// --------------------------------
		// Обработка строк
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
		// Если мы здесь, то это не строка

		// --------------------------------
		// Если встретили символ комментария, ...
		if ( ch == ';' )
		{
			// ...то прерываем цикл, не обрабатывая остаток строки
			break;
		}

		// --------------------------------
		// Если встретили что-то из русского алфавита, сообщим об ошибке
		if ( ((unsigned char)ch) > 127 )
		{
			ReportChar( ch, nLineNumber );
		}

		// --------------------------------
		// Если встретили пробельный символ, то лексема окончена
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
		// Встретили разделитель, кроме \' и \"
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
				// Начинаем набирать ключевое слово или идентификатор
				lexeme.lexType = ltKeyWord;
			}

			// --------------------------------
			else if ( isdigit( ch ) )
			{
				// Начинаем набирать число
				lexeme.lexType = ltNumber;
				bHexCharFound = 0;
				bHexEndFound = 0;
			}

			// --------------------------------
			else
			{
				// Непонятно, что это такое
				ReportChar( ch, nLineNumber );
			}
		}

		// --------------------------------
		// Проверка корректности числа
		if ( lexeme.lexType == ltNumber )
		{
			// Нашли x16 символ?
			if ( isxdigit( ch ) && !isdigit( ch ) )
			{
				bHexCharFound = 1;
			}

			else if ( ch == 'h' || ch == 'H' )
			{
				if ( bHexEndFound )
				{
					// 'h' встречен дважды
					ReportNumber( lexeme.strSource + ch, nLineNumber );
				}

				bHexEndFound = 1;
			}
		}

		lexeme.strSource += ch;

	}	// for (...)

	// --------------------------------
	// Положим последнюю лексему
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
	// Приведение лексем ltKeyWord к нижнему регистру
	// Приведение лексем ltNumber к нижнему регистру
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
// Для перевода номера строки в char[]
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
