#pragma once
#include "ProgramInfo.h"
#include <string>

////////////////////////////////////////////////////////////////////////////////
class COutputFormatter
{
public:
	virtual void SaveAs ( const CProgramInfo& pi, const std::string& strSaveTo ) = 0;
};

////////////////////////////////////////////////////////////////////////////////
class CCPPFormatter : public COutputFormatter
{
public:
	void SaveAs ( const CProgramInfo& pi, const std::string& strSaveTo );
};

////////////////////////////////////////////////////////////////////////////////
class CSourceFormatter : public COutputFormatter
{
public:
	void SaveAs ( const CProgramInfo& pi, const std::string& strSaveTo );
};

////////////////////////////////////////////////////////////////////////////////
