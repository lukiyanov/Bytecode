#include "Reference.h"
#include "ReportAnError.h"
#include "RefType.h"
#include "Command.h"

CReference::CReference(void):
	bImplemented( 0 ), pType( 0 ), bUsed( 0 ), nLine( -1 )
{
}

CReference::CReference( std::string name ):
	bImplemented( 0 ), pType( 0 ), strName( name ), bUsed( 1 ), nLine( -1 )
{
}

CReference::~CReference(void)
{
	delete pType;
}

CReference::CReference( const CLexLine& line, CCommand** pCmd ):
	bUsed( 0 ), bImplemented( 1 ), nLine( line.nLineNumber ), pType( 0 )
{
	const int size = line.vct.size();

	if ( line.vct[0].strSource == "label" )
	{
		pType = CLabel::Construct( line, 0 );
		strName = line.vct[1].strSource;
	}

	//------------------------------------------------------------------------------
	// Переменная?
	else if ( line.vct[0].lexType == ltKeyWord )
	{
		// Далее должен идти тип
		pType = ConstructType( line, 1 );

		strName = line.vct[0].strSource;
	}

	if ( pType )
	{
		// Теперь зарезервируем место под переменную
		(*pCmd) = new CCommand();
		(*pCmd)->nLine = line.nLineNumber;
		(*pCmd)->pRefVar = this;
		(*pCmd)->pTSArray = 0;
		this->pPlaceCmd = *pCmd;
	}
	else
		throw 0;
}
