#pragma once
#include <fstream>
#include <vector>
#include <string>

//------------------------------------------------------------------------------
enum ELexemeType
{
	ltKeyWord, ltNumber, ltString, ltDivider, ltUnknown
};

//------------------------------------------------------------------------------
struct CLexeme
{
	std::string strSource;
	ELexemeType lexType;
	unsigned long int nNumber;
public:
	CLexeme():nNumber(0){}
};

//------------------------------------------------------------------------------
struct CLexLine
{
	int nLineNumber;
	std::vector< CLexeme > vct;
	bool ParseString( const std::string& strLine );
};

//------------------------------------------------------------------------------
class CLexThread
{
public:
	CLexThread( std::ifstream& fsIn );
	~CLexThread(void);

	inline bool HasTokens();
	CLexLine& GetLine();
	void ParseNextLine();

private:
	CLexLine line;
	std::ifstream& m_fsIn;
	bool bGoodBit;
};

//------------------------------------------------------------------------------
inline bool CLexThread::HasTokens()
{
	return bGoodBit;
}

//------------------------------------------------------------------------------
