#include "Formatters.h"
#include <fstream>
using namespace std;

//------------------------------------------------------------------------------
exception CannotOpen( const string& strFile )
{
	string str = "Cannot open the file: \"";
	str += strFile;
	str += "\"";
	return exception( str.c_str() );
}

//------------------------------------------------------------------------------
void CCPPFormatter::SaveAs ( const CProgramInfo& pi, const string& strSaveTo )
{
	// TODO: проверить корректность работы

	string cpp = strSaveTo + ".cpp";

	ofstream fout( cpp.c_str() );

	if ( !fout )
		throw CannotOpen( cpp );

	fout
		<< "#include \"Interpreter.h\"\n\n"
		<< "interpreter::byte aCode[] = {\n";

	for ( int i = 0, size = pi.vctCode.size(); i < size; i++ )
	{
		fout
			<< (int) pi.vctCode[i];

		if ( i != size - 1 )
		{
			fout
				<< ',';

			if ( i && !( (i + 1) % 8) )
			{
				fout << '\n';
			}
			else
			{
				fout << ' ';
			}
		}
	}

	fout
		<< "\n};\n\n"
		<< "void SetInterpreterExtern( interpreter::CInterpreter& InterASM )\n{\n";

	for ( int i = 0, size = pi.vctImports.size(); i < size; i++ )
	{
		fout
			<< "\tInterASM.RegisterExternalFunction( \""
			<< pi.vctImports[i].strModule
			<< "\", \""
			<< pi.vctImports[i].strName
			<< "\" );\n";
	}

	fout << "\n";

	for ( int i = 0, size = pi.vctExports.size(); i < size; i++ )
	{
		fout
			<< "\tInterASM.RegisterCallback( "
			<< pi.vctExports[i].dwOffset
			<< ", "
			<< pi.vctExports[i].dwStackSize
			<< " );\n";
	}

	fout
		<< "}";

	fout.flush();
	fout.close();
}

//------------------------------------------------------------------------------
void CSourceFormatter::SaveAs ( const CProgramInfo& pi, const string& strSaveTo )
{
	// TODO: проверить корректность работы

	string bytecode = strSaveTo + ".bytecode";
	string strExtern  = strSaveTo + ".extern";


	{
		ofstream fout( bytecode.c_str(), ios::out | ios::binary );

		if ( !fout )
			throw CannotOpen( bytecode );

		fout.write( &(pi.vctCode[0]), pi.vctCode.size() );
		fout.flush();
		fout.close();
	}

	{
		ofstream fout( strExtern.c_str(), ios::out );

		if ( !fout )
			throw CannotOpen( strExtern );

		fout << pi.vctExports.size() << endl;
		for (int i = 0, size = pi.vctExports.size(); i < size; i++)
		{
			fout << pi.vctExports[i].dwOffset << ' ' << pi.vctExports[i].dwStackSize << endl;
		}

		fout << pi.vctImports.size() << endl;
		for (int i = 0, size = pi.vctImports.size(); i < size; i++)
		{
			fout << pi.vctImports[i].strModule.c_str() << ' ' << pi.vctImports[i].strName.c_str() << endl;
		}

		fout.flush();
		fout.close();
	}
}

//------------------------------------------------------------------------------
