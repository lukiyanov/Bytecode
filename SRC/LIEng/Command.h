#pragma once
#include "typedefs.h"
#include "VMState.h"

namespace interpreter {

	//------------------------------------------------------------------------------
	typedef void (*CpfnExecute)(CVMState& vmState);
	enum ECOP;

	//------------------------------------------------------------------------------
	struct CCommand {
	public:
		ECOP COP;
		CpfnExecute pfn;
	public:
		//--------------------------------------------------------------------------
		CCommand():
		  COP((ECOP)0), pfn(0)
		{}
		//--------------------------------------------------------------------------
		CCommand( const ECOP cop, CpfnExecute pf ):
		  COP(cop), pfn(pf)
		{}
		//--------------------------------------------------------------------------
		void Execute(CVMState& vmState)
		{ pfn(vmState); }
		//--------------------------------------------------------------------------
		CCommand& operator = (const CCommand& cmd)
		{ COP = cmd.COP; pfn = cmd.pfn; return *this; }
	};

	//------------------------------------------------------------------------------
	extern const CCommand aRegisteredCommands[];

}
