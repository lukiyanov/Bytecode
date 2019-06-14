#pragma once
#include "RTTI.h"
#include "LexThread.h"

#include <map>

class  CReference;
struct CProgramInfo;
struct СRefType;

class CArgument : public CRuntimeObject
{
protected:
	int bits;

public:
	virtual void Generate( CProgramInfo& pi ) = 0;

	// Размер аргумента в команде
	virtual unsigned long GetBits() = 0;

	// Размер объекта, на который указывает аргумент,
	// если это переменная или указатель
	virtual unsigned long GetTargetBits() = 0;
};

class CVariableArg : public CArgument
{
	DECLARE_DYNAMIC( CVariableArg )

public:
	CReference* pVariableUsed;	// 0 -> используется dwAddress

protected:
//	int bits;					// 0 если не используется
	unsigned long int dwAddress;

public:
	static CVariableArg* Construct( const CLexLine& args, std::map< std::string, CReference* >& mapRef );
	static CVariableArg* SimpleConstruct( const CLexLine& line, const int offset, const int backward, std::map< std::string, CReference* >& mapRef );

	void Generate( CProgramInfo& pi );
	unsigned long GetBits();
	unsigned long GetTargetBits();
};

class CValueArg : public CArgument
{
	DECLARE_DYNAMIC( CValueArg )

private:
	unsigned long int nValue;

public:
	static CValueArg* Construct( const CLexLine& args, std::map< std::string, CReference* >& mapRef );
	void Generate( CProgramInfo& pi );
	unsigned long GetBits();
	unsigned long GetTargetBits();
};

class CPointerArg : public CArgument
{
	DECLARE_DYNAMIC( CPointerArg )

private:
	signed short int offset;
	CReference* pVariableUsed;
	unsigned int nLine;

public:
	static CPointerArg* Construct( const CLexLine& args, std::map< std::string, CReference* >& mapRef );
	void Generate( CProgramInfo& pi );
	unsigned long GetBits();
	unsigned long GetTargetBits();
};

CArgument* ConstructArg ( const CLexLine& args, std::map< std::string, CReference* >& mapRef );
