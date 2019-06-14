#pragma once
#include "LexThread.h"

////////////////////////////////////////////////////////////////////////////////
class CVariableArg;
struct CProgramInfo;

// ��������� ��������?

////////////////////////////////////////////////////////////////////////////////
class CRefType
{
	// CVariableArg ����� ����� ��������� CRefType
	friend class CVariableArg;

public:
	unsigned int nAddr;
	unsigned int nBits;

public:
	CRefType(void);
	virtual ~CRefType(void);

	// ���������� 0 ��� �������
	static CRefType* Construct( const CLexLine& line, const int offset )
	{ return 0; }
	//
	virtual void Generate( CProgramInfo& pi ) = 0;
	virtual unsigned long GetTranslatedSize() { return nBits / 8; }
};

////////////////////////////////////////////////////////////////////////////////
class CByte : public CRefType
{
public:
	unsigned char byInitial;
	std::string strArray;

public:
	static CByte* Construct( const CLexLine& line, const int offset );
	void Generate( CProgramInfo& pi );
	virtual unsigned long GetTranslatedSize();

private:
	CByte();
};

////////////////////////////////////////////////////////////////////////////////
class CWord : public CRefType
{
public:
	unsigned short wInitial;

public:
	static CWord* Construct( const CLexLine& line, const int offset );
	void Generate( CProgramInfo& pi );

private:
	CWord();
};

////////////////////////////////////////////////////////////////////////////////
class CDWord : public CRefType
{
public:
	unsigned long dwInitial;

public:
	static CDWord* Construct( const CLexLine& line, const int offset );
	void Generate( CProgramInfo& pi );

private:
	CDWord();
};

////////////////////////////////////////////////////////////////////////////////
class CStructure : public CRefType
{
public:
//	static CRefType* Construct( const CLexLine& line, const int offset );
	// ����� ���� �����-����� �����...
};

////////////////////////////////////////////////////////////////////////////////
//
CRefType* ConstructType( const CLexLine& line, const int offset );

////////////////////////////////////////////////////////////////////////////////
class CLabel : public CRefType
{
public:
	static CLabel* Construct( const CLexLine& line, const int offset );
	void Generate( CProgramInfo& pi );
	unsigned long GetTranslatedSize();

private:
	CLabel();
};

////////////////////////////////////////////////////////////////////////////////
