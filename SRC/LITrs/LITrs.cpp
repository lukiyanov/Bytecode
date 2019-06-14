#include <string>
#include <fstream>
#include <iostream>
using namespace std;

#include "LexThread.h"
#include "Translator.h"
#include "Formatters.h"

const int nVersionLow  = 8;
const int nVersionHigh = 0;

////////////////////////////////////////////////////////////////////////////////
void ShowIntro();
void ShowUsage();
bool ParseCmdLine(/* IN: */ const int argc, char* argv[],
				  /* OUT:*/ string& infile, string& outfile, COutputFormatter** pFormatter = 0 );

////////////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[])
{
	//------------------------------------------------------------------------------
	// Выводим версию
	ShowIntro();

	//------------------------------------------------------------------------------
	// Парсинг командной строки
	string strInFile, strOutFile;
	COutputFormatter *pFormatter = 0;
	if ( ParseCmdLine( argc, argv, strInFile, strOutFile, &pFormatter ) )
	{
		return 1;
	}

	//------------------------------------------------------------------------------
	ifstream fInput( strInFile.c_str() );
	if ( !fInput )
	{
		cout << "Can't open input file \"" << strInFile << "\"" << endl;
		return 2;
	}

	//------------------------------------------------------------------------------
	CTranslator trs;
	try
	{
		CLexThread lexThread( fInput );

		// Цикл парсинга
		while ( lexThread.HasTokens() )
		{
			lexThread.ParseNextLine();
			trs.Translate(lexThread.GetLine());
		}

		// Подставляем значения в ссылки и генерируем выходные файлы
		trs.Generate();
		pFormatter->SaveAs( trs.GetProgramInfo(), strOutFile );
		delete pFormatter;
	}
	catch ( exception& e )
	{
		cout
			<< "Error: "
			<< e.what()
			<< endl;

		return 3;
	}

	//------------------------------------------------------------------------------
	fInput.close();
	cout
		<< "Done. Total code size is: "
		<< trs.GetProgramInfo().vctCode.size()
		<< endl;

	return 0;
}

////////////////////////////////////////////////////////////////////////////////
// Выводим информацию о версии
void ShowIntro()
{
	cout
		<< "LI Translator v"
		<< nVersionHigh
		<< '.'
		<< nVersionLow
		<< " by Lukyanov"
		<< endl;

	cout
		<< "Current version builded at: "
		<< __DATE__ <<
		", "
		<< __TIME__
		<< endl
		<< endl;
}

////////////////////////////////////////////////////////////////////////////////
// Вывод сообщения о формате командной строки
void ShowUsage()
{
	cout
		<< "Error: invalid command line format" << endl
		<< endl
		<< "Syntax description:" << endl
		<< "{variant1|variant2|...} - one of the following content item" << endl
		<< "[...] - unnecessary item" << endl
		<< endl
		<< "Command line format:" << endl
		<< "-in  input_file_name" << endl
		<< "[-out output_file_name]" << endl
		<< "\toutput_file_name - Output file name without externition," << endl
		<< "\tas default is equal input_file_name." << endl
		<< "[-as { fileset | cpp }]" << endl
		<< "\tfileset - generate two files \".bytecode\" and \".extern\"," << endl
		<< "\tthis is default." << endl
		<< "\tcpp - generate \".cpp\" file with array of bytes(byte-code) and" << endl
		<< "\t \"SetInterpreterExtern\" function." << endl;
}

////////////////////////////////////////////////////////////////////////////////
// Парсинг командной строки, возвращает 0, если все в порядке
bool ParseCmdLine(
	/* IN: */ const int argc, char* argv[],
	/* OUT:*/ string& infile, string& outfile, COutputFormatter** pFormatter
	)
{
	enum { fileset, cpp }
	asType = fileset;

	try {

		if ( argc < 2)
		{
			throw 0;
		}

		enum ELastKey { keyNone, keyIn, keyOut, keyAs };
		ELastKey lastKey = keyNone;
		for (int i = 1; i < argc; i++)
		{
			switch ( lastKey )
			{
			case keyIn:
				lastKey = keyNone;
				infile = argv[i];
				continue;

			case keyOut:
				lastKey = keyNone;
				outfile = argv[i];
				continue;

			case keyAs:
				lastKey = keyNone;
				if		( !strcmp(argv[i], "fileset")  )
				{
					asType = fileset;
				}				
				else if	( !strcmp(argv[i], "cpp")  )
				{
					asType = cpp;
				}				
				else
				{
					throw 1;
				}				
				continue;
			}

			// Ждем-с флаги
			if		( !strcmp(argv[i], "-in")  )
			{
				lastKey = keyIn;
			}
			else if	( !strcmp(argv[i], "-out") )
			{
				lastKey = keyOut;
			}
			else if	( !strcmp(argv[i], "-as")  )
			{
				lastKey = keyAs;
			}
			else
			{
				throw 2;
			}
		}

		if ( infile == "" )
			throw 3;

		if ( outfile == "" )
			outfile = infile;

		switch ( asType )
		{
		case fileset:
			*pFormatter = new CSourceFormatter;
			break;

		case cpp:
			*pFormatter = new CCPPFormatter;
			break;

		default:
			throw 0xff;
		}
	}
	catch (...)
	{
		ShowUsage();
		return 1;	// Invalid format
	}

	return 0;	// Ok
}

////////////////////////////////////////////////////////////////////////////////
