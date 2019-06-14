////////////////////////////////////////////////////////////////////////////////

// Включить все проверки корректности
//#define ALLOW_CHECKS // в настройках проекта

////////////////////////////////////////////////////////////////////////////////

#include "Interpreter.h"
#include "Exceptions.h"

#include <string>
#include <fstream>
using namespace std;

#include <windows.h>

//#include "Trace.h"
//#include "Profile.h"

wchar_t szAppName[] = L"LI Interpreter";
////////////////////////////////////////////////////////////////////////////////
interpreter::byte* pCode;

////////////////////////////////////////////////////////////////////////////////
int WINAPI wWinMain( HINSTANCE, HINSTANCE, LPWSTR lpCmdLine, int )
{
	// Проверка по ALLOW_CHECKS "съедает" ~22% производительности

	interpreter::CInterpreter InterASM;

	//------------------------------------------------------------------------------
	// Парсинг командной строки
	//------------------------------------------------------------------------------

	int argc = 0;
	LPWSTR* argv = CommandLineToArgvW( GetCommandLineW(), &argc );
	if ( !argv || argc < 2 )
	{
		MessageBox( 0, L"В командной строке не указан файл,\nподлежащий исполнению (без расширения)", szAppName, MB_ICONWARNING );
		return 1;
	}

	wstring strBytecode = argv[1], strExtern = argv[1];
	strBytecode += L".bytecode";
	strExtern   += L".extern";

	LocalFree( argv );

	//------------------------------------------------------------------------------
	// Загрузка файла байт-кода
	//------------------------------------------------------------------------------
	DWORD dwSize = 0;
	{
		// Находим размер файла

		HANDLE hFile = (HANDLE)CreateFile( strBytecode.c_str(), FILE_READ_ATTRIBUTES, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL );

		if ( hFile == INVALID_HANDLE_VALUE )
		{
			strBytecode = wstring( L"Не удалось открыть входной файл \"" ) + strBytecode + L"\"";
			MessageBox( 0, strBytecode.c_str(), szAppName, MB_ICONSTOP );
			return 2;
		}
		dwSize = GetFileSize( hFile, 0 );
		CloseHandle( hFile );

		// Считываем файл
		pCode = new interpreter::byte[ dwSize ];

		ifstream fin;
		fin.open( strBytecode.c_str(), ios::in | ios::binary );
		fin.read( (char*)pCode, dwSize );
		fin.close();
	}

#ifdef ALLOW_CHECKS
	try
	{
#endif
		// -------
		InterASM.InitSession( pCode, dwSize, 0 );

		//------------------------------------------------------------------------------
		// Загрузка файла списка импорта/экспорта
		//------------------------------------------------------------------------------
		{
			ifstream fin;
			fin.open( strExtern.c_str(), ios::in );
			if ( !fin )
			{
				strExtern = wstring() + L"Не удалось открыть входной файл \"" + strExtern + L"\"";
				MessageBox( 0, strExtern.c_str(), szAppName, MB_ICONSTOP );
				return 3;
			}

			// Экспорт
			int n;
			fin >> n;
			unsigned offset, stack;
			for (int i = 0; i < n; i++)
			{
				fin >> offset;
				fin >> stack;
				InterASM.RegisterCallback( offset, stack );
			}

			// Импорт
			fin >> n;
			string module, name;
			for (int i = 0; i < n; i++)
			{
				fin >> module;
				fin >> name;
				InterASM.RegisterExternalFunction( module.c_str(), name.c_str() );
			}

			fin.close();
		}

		{
//			rdtsc();
//			uprof overhead = rdtsc();
//			overhead = rdtsc() - overhead;
//			uprof start = rdtsc();

			InterASM.Run();
			

//			uprof stop = rdtsc();
//			uprof result = stop - start - overhead - overhead;
//			result = result;
		}

#ifdef ALLOW_CHECKS
	}
	catch (interpreter::CInterpretException& e)
	{
		MessageBoxA( 0, e.Message().c_str(), "exception", MB_ICONWARNING);
	}
	catch (std::string str)
	{
		MessageBoxA( 0, str.c_str(), "exception", MB_ICONWARNING);
	}
#endif

	delete[] pCode;

	return 0;
}

////////////////////////////////////////////////////////////////////////////////
