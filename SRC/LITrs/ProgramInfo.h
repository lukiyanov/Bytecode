#pragma once
#include <vector>
#include <string>

#include "trstypedefs.h"

//------------------------------------------------------------------------------
struct CExportProc
{
	std::string strDefinitionName;
	std::string strLabelName;

	dword dwId;
	dword dwStackSize;
	dword dwOffset;
};

//------------------------------------------------------------------------------
struct CImportProc
{
	std::string strDefinitionName;

	dword id;
	std::string strModule;
	std::string strName;
};

//------------------------------------------------------------------------------
struct CProgramInfo
{
	std::vector< char > vctCode;
	std::vector< CImportProc > vctImports;
	std::vector< CExportProc > vctExports;
};

//------------------------------------------------------------------------------
